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
# functions #
#############
def match_pfx(uni_word, morphs):
  """
  try to match with word prefix and head of morpheme list
  @param  uni_word  unicode word
  @param  morphs    morpheme list
  @return           (match character length in word, match morpheme number in morpheme list) tuple
  """
  uni_morph = unicode(morphs[0].lex, 'UTF-8')
  if uni_word.startswith(uni_morph):
    return len(uni_morph), 1
  return -1, -1


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
      logging.debug(word)
      uni_word = unicode(word.raw, 'UTF-8')
      morphs = word.morphs
      while uni_word:
        word_match_len, morph_match_num = match_pfx(uni_word, morphs)
        if word_match_len == -1:
          logging.debug(u'    %s: %d, %d', uni_word, word_match_len, morph_match_num)
          break
        syllables = uni_word[:word_match_len].encode('UTF-8')
        morphemes = ' + '.join([str(morph) for morph in morphs[:morph_match_num]])
        print >> fout, '%s\t%s' % (syllables, morphemes)
        uni_word = uni_word[word_match_len:]
        morphs = morphs[morph_match_num:]


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
