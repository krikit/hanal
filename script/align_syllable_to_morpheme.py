#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
align syllables to morphemes in word(EoJeol)
__author__      krikit
__email__       krikit@naver.com
__date__        creation: 2014-05-27, modification: 2014-05-27
__copyright__   nobody. feel free to use, copy and modify
"""


###########
# imports #
###########
import argparse
import logging
import sys

# for decomposing syllables into consonants and vowels
# http://biohackers.net/wiki/PythonAndHangul#head-4788cfe894c55238b6133994c6ab356302f63ec8
import hangul
import sejong_corpus


###########
# options #
###########
IS_SPOKEN = False


#############
# constants #
#############
_JONG_TO_CHO = {
  u'ㄴ': u'\u11ab',
  u'ㄹ': u'\u11af',
}


#############
# functions #
#############
def decompose(uni_word):
  """
  decompose syllables into consonants and vowels
  @param  uni_word  word
  @return           decomposed word
  """
  if isinstance(uni_word, str):
    uni_word = unicode(uni_word, 'UTF-8')
  chars = []
  for uni_char in uni_word:
    if not hangul.ishangul(uni_char):
      chars.append(uni_char)
      continue
    chars.extend([_JONG_TO_CHO.get(alpha, alpha) for alpha in hangul.split(uni_char) if alpha])
  return (u''.join(chars)).encode('UTF-8')


def to_hex(text):
  """
  to hex code string
  @param  text  text
  @return       hex coded string
  """
  return ' '.join([hex(ord(char)) for char in unicode(text, 'UTF-8')])


def match_pfx(uni_word, morphs):
  """
  try to match with word prefix and head of morpheme list
  @param  uni_word  unicode word
  @param  morphs    morpheme list
  @return           (match character length in word, match morpheme number in morpheme list) tuple
  """
  uni_morph = unicode(morphs[0].lex, 'UTF-8')
  if uni_word.startswith(uni_morph):    # just one morpheme starts with word
    return len(uni_morph), 1
  if len(morphs) == 1:    # only one morpheme is left
    return 1, len(uni_word)
  for i in range(2, len(morphs)+1):
    submorphs = ''.join([morph.lex for morph in morphs[:i]])
    submorphs_dec = decompose(submorphs)
    for k in range(1, len(unicode(submorphs, 'UTF-8'))):
      word_dec = decompose(uni_word[:k])
      # logging.debug('  %s(%s:%s) <- %s(%s:%s)', uni_word[:k].encode('UTF-8'), word_dec, to_hex(word_dec),
      #     submorphs, submorphs_dec, to_hex(submorphs_dec))
      if word_dec == submorphs_dec:
        return k, i
  morphs_str = ' + '.join([str(morph) for morph in morphs])
  logging.debug('PFX: %s(%s): %s', uni_word.encode('UTF-8'), decompose(uni_word), morphs_str)
  return -1, -1


def match_sfx(uni_word, morphs):
  """
  try to match with word suffix and last of morpheme list
  @param  uni_word  unicode word
  @param  morphs    morpheme list
  @return           (match character length in word, match morpheme number in morpheme list) tuple
  """
  uni_morph = unicode(morphs[-1].lex, 'UTF-8')
  if uni_word.endswith(uni_morph):    # just one morpheme ends with word
    return len(uni_morph), 1
  for i in range(-2, -(len(morphs)+1), -1):
    submorphs = ''.join([morph.lex for morph in morphs[i:]])
    submorphs_dec = decompose(submorphs)
    for k in range(-1, -len(unicode(submorphs, 'UTF-8')), -1):
      word_dec = decompose(uni_word[k:])
      # logging.debug('  %s(%s:%s) <- %s(%s:%s)', uni_word[k:].encode('UTF-8'), word_dec, to_hex(word_dec),
      #     submorphs, submorphs_dec, to_hex(submorphs_dec))
      if word_dec == submorphs_dec:
        return -k, -i
  morphs_str = ' + '.join([str(morph) for morph in morphs])
  logging.debug('SFX: %s(%s): %s', uni_word.encode('UTF-8'), decompose(uni_word), morphs_str)
  return -1, -1


def log_not_aligned(word, forward_pairs, sandwitch_word, sandwitch_morphs, backward_pairs):
  """
  logging not aligned word
  @param  word              word
  @param  forward_pairs     forward pairs
  @param  sandwitch_word    sandwitch word
  @param  sandwitch_morphs  sandwitch morphemes
  @param  backward_pairs    backward pairs
  """
  if isinstance(word, unicode):
    word = word.encode('UTF-8')
  if isinstance(sandwitch_word, unicode):
    sandwitch_word = sandwitch_word.encode('UTF-8')
  logging.error('Not aligned word: [%s]', word)
  for word_str, morphs in forward_pairs:
    if isinstance(word_str, unicode):
      word_str = word_str.encode('UTF-8')
    morph_str = ' + '.join([str(morph) for morph in morphs])
    logging.error('  Forward:   [%s]\t%s', word_str, morph_str)
  if sandwitch_word and sandwitch_morphs:
    logging.error('  Sandwitch: [%s]\t%s', sandwitch_word, ' + '.join([str(morph) for morph in sandwitch_morphs]))
  elif sandwitch_word:
    logging.error('  SW Word:   [%s]', sandwitch_word)
  else:
    logging.error('  SW Morphs: %s', ' + '.join([str(morph) for morph in sandwitch_morphs]))
  for word_str, morphs in backward_pairs:
    if isinstance(word_str, unicode):
      word_str = word_str.encode('UTF-8')
    morph_str = ' + '.join([str(morph) for morph in morphs])
    logging.error('  Backward:  [%s]\t%s', word_str, morph_str)


def print_aligned(fout, pairs):
  """
  print aligned (word, morpheme) pairs
  @param  fout   output file
  @param  pairs  (word, morpheme) pairs
  """
  for word, morphs in pairs:
    if isinstance(word, unicode):
      word = word.encode('UTF-8')
    morph_str = ' + '.join([str(morph) for morph in morphs])
    print >> fout, '%s\t%s' % (word, morph_str)


########
# main #
########
def main(fin_names, fout):
  """
  align syllables to morphemes in word(EoJeol)
  @param  fin_names   list of input files
  @param  fout    output file
  """
  for sent in sejong_corpus.load(IS_SPOKEN, fin_names):
    for word in sent.words:
      # logging.debug(word)
      uni_word = unicode(word.raw, 'UTF-8')
      morphs = word.morphs
      forward_pairs = []
      while uni_word and morphs:
        word_match_len, morph_match_num = match_pfx(uni_word, morphs)
        if word_match_len == -1:
          break
        forward_pairs.append((uni_word[:word_match_len], morphs[:morph_match_num]))
        uni_word = uni_word[word_match_len:]
        morphs = morphs[morph_match_num:]
      backward_pairs = []
      while uni_word and morphs:
        word_match_len, morph_match_num = match_sfx(uni_word, morphs)
        if word_match_len == -1:
          break
        backward_pairs.insert(0, (uni_word[-word_match_len:], morphs[-morph_match_num:]))
        uni_word = uni_word[:-word_match_len]
        morphs = morphs[:-morph_match_num]
      sandwitch_pairs = []
      if uni_word and morphs:
        if len(uni_word) == 1 or len(morphs) == 1:
          sandwitch_pairs.append((uni_word, morphs))
        else:
          log_not_aligned(word.raw, forward_pairs, uni_word, morphs, backward_pairs)
          continue
      elif uni_word:
        log_not_aligned(word.raw, forward_pairs, uni_word, morphs, backward_pairs)
        continue
      elif morphs:
        log_not_aligned(word.raw, forward_pairs, uni_word, morphs, backward_pairs)
        continue
      print_aligned(fout, forward_pairs + sandwitch_pairs + backward_pairs)


if __name__ == '__main__':
  _PARSER = argparse.ArgumentParser(description='align syllables to morphemes in word(EoJeol)')
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
