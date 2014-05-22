#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
make word(EoJeol) per line formatted corpus from Sejong tagged corpus
@author   krikit@naver.com
@date     Creation: 2014-05-22, Modification: 2014-05-22
"""


###########
# imports #
###########
import argparse
import codecs
import logging
import re
import sys


###########
# options #
###########
IS_SPOKEN = False


#############
# constants #
#############
_WRITTEN_SENT_OPEN_TAGS = [u'<head>', u'<p>',]
_WRITTEN_SENT_CLOSE_TAGS = [tag[0] + u'/' + tag[1:] for tag in _WRITTEN_SENT_OPEN_TAGS]

_OPEN_TAGS_IN_SPOKEN_SENT = [
    u'<anchor', u'<applauding', u'<dia', u'<event', u'<kinesics', u'<latching', u'<laughing', u'<note', u'<pause',
    u'<quotation', u'<read', u'<reading', u'<singing', u'<trunc', u'<unclear', u'<vocal',
]
_CLOSE_TAGS_IN_SPOKEN_SENT = [tag[0] + u'/' + tag[1:] for tag in _OPEN_TAGS_IN_SPOKEN_SENT]
_TAGS_IN_SPOKEN_SENT = _OPEN_TAGS_IN_SPOKEN_SENT + _CLOSE_TAGS_IN_SPOKEN_SENT

_OPEN_TAGS_IN_WRITTEN_SENT = set([u'<date>',])
_CLOSE_TAGS_IN_WRITTEN_SENT = set([tag[0] + u'/' + tag[1:] for tag in _OPEN_TAGS_IN_WRITTEN_SENT])
_TAGS_IN_WRITTEN_SENT = _OPEN_TAGS_IN_WRITTEN_SENT | _CLOSE_TAGS_IN_WRITTEN_SENT

_LINE_ID_PTN = re.compile(u'[0-9A-Z_]{4}\\d{4}-\\d{7,8}')


#############
# functions #
#############
def clean_spoken(sent):
  """
  clean spoken sentence
  @param  sent  sentence
  @return       cleaned sentence
  """
  sent = re.sub(r'<anchor [^>]+/?>', u'', sent)
  sent = re.sub(r'<event [^>]+/?>', u'', sent)
  sent = re.sub(r'<kinesics [^>]+/?>', u'', sent)
  sent = re.sub(r'<p>.*</p>', u'', sent)
  sent = re.sub(r'<pause [^>]+/?>', u'', sent)
  sent = re.sub(r'<phon>.*</phon>', u'', sent)
  sent = re.sub(r'<sound [^>]+/?>', u'', sent)
  sent = re.sub(r'<vocal [^>]+/?>', u'', sent)
  sent = sent.replace(u'::', u'')
  for open_tag in _OPEN_TAGS_IN_SPOKEN_SENT:
    open_tag += u'>'
    sent = sent.replace(open_tag, u'')
    open_tag += u'/>'
    sent = sent.replace(open_tag, u'')
  for close_tag in _CLOSE_TAGS_IN_SPOKEN_SENT:
    sent = sent.replace(close_tag + u'>', u'')
  return sent.strip()

def make_raw_sent(words):
  """
  make raw sentence from word list
  @param  words   word list
  @return         raw sentence
  """
  return ' '.join([word.split('\t')[0] for word in words])


def lines(fin):
  """
  return lines (generator)
  @param  fin   input file
  @yield        lines
  """
  for uni_line in fin:
    if uni_line[0] == u'\uFEFF':
      uni_line = uni_line[1:]
    if _LINE_ID_PTN.match(uni_line):
      uni_line = uni_line[uni_line.index('\t')+1:]
    yield uni_line


def is_sent_open(uni_line):
  """
  whether sentence opening or not
  @param  uni_line  line
  @return           whether opening or not
  """
  if IS_SPOKEN:
    return uni_line.startswith(u'<s ')
  else:
    return uni_line in _WRITTEN_SENT_OPEN_TAGS


def is_sent_close(uni_line):
  """
  whether sentence closing or not
  @param  uni_line  line
  @return       whether closing or not
  """
  if IS_SPOKEN:
    return uni_line.startswith(u'</s>')
  else:
    return uni_line in _WRITTEN_SENT_CLOSE_TAGS


def is_tag_in_sent(uni_line):
  """
  whether tag in sentence or not
  @param  uni_line  line
  @return       whether tag or not
  """
  if IS_SPOKEN:
    if uni_line[0] != u'<':
      return False
    for inner_tag in _TAGS_IN_SPOKEN_SENT:
      if uni_line.startswith(inner_tag):
        return True
    return False
  else:
    return uni_line in _TAGS_IN_WRITTEN_SENT


########
# main #
########
def main(fin_names, fout):
  """
  make word(EoJeol) per line formatted corpus from Sejong tagged corpus
  @param  fin_names   list of input files
  @param  fout    output file
  """
  for fin_name in fin_names:
    logging.info('%s', fin_name)

    num_sents = 0
    words = None
    fin = codecs.open(fin_name, encoding='UTF-16LE')
    for line_num, uni_line in enumerate(lines(fin), start=1):
      uni_line = uni_line.strip()
      if not uni_line:
        continue
      if is_sent_open(uni_line):
        words = []
        continue
      elif is_sent_close(uni_line):
        if words:
          print >> fout, '#', make_raw_sent(words)
          print >> fout, '%s\n' % ('\n'.join(words))
          num_sents += 1
        words = None
        continue
      elif words is None:
        continue
      cols = uni_line.split(u'\t')
      if len(cols) != 2:
        if is_tag_in_sent(uni_line):
          continue
        logging.error('Invalid line(%d): %s', line_num, uni_line)
        words = None
        continue
      if IS_SPOKEN:
        cols[0] = clean_spoken(cols[0])
        cols[1] = u' + '.join(cols[1].split(u'+'))
      words.append('\t'.join([col.encode('UTF-8') for col in cols]))
    if words:
      logging.error('Last sentence has no closing tag')
    if num_sents == 0:
      logging.warning('%s has no sents', fin_name)


if __name__ == '__main__':
  _PARSER = argparse.ArgumentParser(description='make word(EoJeol) per line formatted corpus from Sejong tagged corpus')
  _PARSER.add_argument('input', help='input files', metavar='FILE', nargs='+')
  _PARSER.add_argument('--is-spoken', help='whether spoken corpus or not', action='store_true')
  _PARSER.add_argument('--output', help='output file <default: stdout>', metavar='FILE',
      type=argparse.FileType('w'), default=sys.stdout)
  _PARSER.add_argument('--log-level', help='set logging level', metavar='LEVEL')
  _PARSER.add_argument('--log-file', help='set log file <default: stderr>', metavar='FILE')
  _ARGS = _PARSER.parse_args()
  _LOG_CFG = {'format':'[%(asctime)-15s] %(levelname)-8s %(message)s', 'datefmt':'%Y-%m-%d %H:%M:%S'}
  if _ARGS.log_level:
    _LOG_CFG['level'] = eval('logging.%s' % _ARGS.log_level.upper())
  if _ARGS.log_file:
    _LOG_CFG['filename'] = _ARGS.log_file
  logging.basicConfig(**_LOG_CFG)    # pylint: disable=W0142
  IS_SPOKEN = _ARGS.is_spoken
  main(_ARGS.input, _ARGS.output)
