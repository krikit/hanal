# -*- coding: utf-8 -*-


"""
Sejong corpus parser
__author__      krikit
__email__       krikit@naver.com
__date__        creation: 2014-05-22, modification: 2014-05-28
__copyright__   nobody. feel free to use, copy and modify
"""


###########
# imports #
###########
import codecs
import logging
import re


#############
# constants #
#############
_WRITTEN_SENT_OPEN_TAGS = ['<head>', '<p>',]
_WRITTEN_SENT_CLOSE_TAGS = [tag[0] + '/' + tag[1:] for tag in _WRITTEN_SENT_OPEN_TAGS]

_OPEN_TAGS_IN_SPOKEN_SENT = [
  '<anchor', '<applauding', '<dia', '<event', '<kinesics', '<latching', '<laughing', '<note', '<pause',
  '<quotation', '<read', '<reading', '<singing', '<trunc', '<unclear', '<vocal',
]
_CLOSE_TAGS_IN_SPOKEN_SENT = [tag[0] + '/' + tag[1:] for tag in _OPEN_TAGS_IN_SPOKEN_SENT]
_TAGS_IN_SPOKEN_SENT = _OPEN_TAGS_IN_SPOKEN_SENT + _CLOSE_TAGS_IN_SPOKEN_SENT

_OPEN_TAGS_IN_WRITTEN_SENT = set(['<date>',])
_CLOSE_TAGS_IN_WRITTEN_SENT = set([tag[0] + '/' + tag[1:] for tag in _OPEN_TAGS_IN_WRITTEN_SENT])
_TAGS_IN_WRITTEN_SENT = _OPEN_TAGS_IN_WRITTEN_SENT | _CLOSE_TAGS_IN_WRITTEN_SENT

_UNI_LINE_ID_PTN = re.compile(u'[0-9A-Z_]{4}\\d{4}-\\d{7,8}')


#########
# types #
#########
class ParseError(Exception):
  """
  error occured while parsing Sejong corpus
  """
  pass


class Sentence(object):
  """
  Sentence
  """
  def __init__(self):
    self.words = []   # word list

  def __str__(self):
    raw_words_str = '# %s' % ' '.join([word.raw_str() for word in self.words])
    words_str = '\n'.join([str(word) for word in self.words])
    return '%s\n%s' % (raw_words_str, words_str)

  @classmethod
  def is_opening(cls, is_spoken, line):
    """
    whether sentence opening or not
    @param  is_spken  whether is spoken
    @param  line      unicode line
    @return           whether opening or not
    """
    if is_spoken:
      return line.startswith('<s ')
    else:
      return line in _WRITTEN_SENT_OPEN_TAGS

  @classmethod
  def is_closing(cls, is_spoken, line):
    """
    whether sentence closing or not
    @param  is_spken  whether is spoken
    @param  line      unicode line
    @return           whether closing or not
    """
    if is_spoken:
      return line.startswith('</s>')
    else:
      return line in _WRITTEN_SENT_CLOSE_TAGS

  @classmethod
  def is_tag_in_sent(cls, is_spoken, line):
    """
    whether tag in sentence or not
    @param  is_spken  whether is spoken
    @param  line      unicode line
    @return           whether tag or not
    """
    if is_spoken:
      if line[0] != '<':
        return False
      for inner_tag in _TAGS_IN_SPOKEN_SENT:
        if line.startswith(inner_tag):
          return True
      return False
    else:
      return line in _TAGS_IN_WRITTEN_SENT


class Word(object):
  """
  Word(EoJeol)
  """
  def __init__(self):
    self.raw = ''   # raw word string
    self.morphs = []    # morpheme list

  def __str__(self):
    return '%s\t%s' % (self.raw, ' + '.join([str(morph) for morph in self.morphs]))

  def raw_str(self):
    """
    return raw string instead of word per line format
    @return   raw word
    """
    return self.raw

  @classmethod
  def parse(cls, is_spoken, line, file_name, line_num):
    """
    parse word(EoJeol) with single line
    @param  is_spoken   whether is spoken
    @param  line        unicode line
    @param  file_name   file name
    @param  line_num    line number
    """
    cols = line.split('\t')
    if len(cols) != 2:
      if Sentence.is_tag_in_sent(is_spoken, line):
        return None
      raise ParseError('Invalid line(%d): %s' % (line_num, line))
    if is_spoken:
      cols[0] = cls.clean_spoken(cols[0])
      cols[1] = ' + '.join(cols[1].split('+'))
    word = Word()
    word.raw = cols[0]
    try:
      word.morphs = [Morph.parse(word_tag) for word_tag in cols[1].split(' + ')]
    except ValueError, val_err:
      logging.error('line: %s', line)
      raise ParseError('%s(%d) %s' % (file_name, line_num, val_err))
    return word

  @classmethod
  def clean_spoken(cls, sent):
    """
    clean spoken sentence
    @param  sent  sentence
    @return       cleaned sentence
    """
    sent = re.sub(r'<anchor [^>]+/?>', '', sent)
    sent = re.sub(r'<event [^>]+/?>', '', sent)
    sent = re.sub(r'<kinesics [^>]+/?>', '', sent)
    sent = re.sub(r'<p>.*</p>', '', sent)
    sent = re.sub(r'<pause [^>]+/?>', '', sent)
    sent = re.sub(r'<phon>.*</phon>', '', sent)
    sent = re.sub(r'<sound [^>]+/?>', '', sent)
    sent = re.sub(r'<vocal [^>]+/?>', '', sent)
    sent = sent.replace('::', '')
    for open_tag in _OPEN_TAGS_IN_SPOKEN_SENT:
      open_tag += '>'
      sent = sent.replace(open_tag, '')
      open_tag += '/>'
      sent = sent.replace(open_tag, '')
    for close_tag in _CLOSE_TAGS_IN_SPOKEN_SENT:
      sent = sent.replace(close_tag + '>', '')
    return sent.strip()


class Morph(object):
  """
  Morpheme
  """
  def __init__(self):
    self.lex = ''   # lexical form
    self.tag = ''   # part-of-speech tag

  def __str__(self):
    return '%s/%s' % (self.lex, self.tag)

  @classmethod
  def parse(cls, token_str):
    """
    parse token string
    @param  token_str   morpheme/tag string
    @return             Morph object
    """
    morph = Morph()
    morph.lex, morph.tag = token_str.rsplit('/', 1)
    return morph


#############
# functions #
#############
def load(is_spoken, paths):
  """
  parse Sejong corpus and return sentences (generator)
  @param  is_spoken   whether spoken corpus or not
  @param  paths       list of file paths
  @yield              Sentence object
  """
  def lines(fin):
    """
    return lines (generator)
    @param  fin   input file
    @yield        lines
    """
    for uni_line in fin:
      if uni_line[0] == u'\uFEFF':
        uni_line = uni_line[1:]
      if _UNI_LINE_ID_PTN.match(uni_line):
        uni_line = uni_line[uni_line.index(u'\t')+1:]
      yield uni_line.encode('UTF-8')

  for fin_name in paths:
    logging.info('%s', fin_name)
    fin = codecs.open(fin_name, encoding='UTF-16LE')

    sent = None
    for line_num, line in enumerate(lines(fin), start=1):
      line = line.strip()
      if not line:
        continue
      if Sentence.is_opening(is_spoken, line):
        sent = Sentence()
        continue
      elif Sentence.is_closing(is_spoken, line):
        if sent:
          yield sent
        sent = None
        continue
      elif not sent:
        continue
      word = Word.parse(is_spoken, line, fin_name, line_num)
      if word:
        sent.words.append(word)
