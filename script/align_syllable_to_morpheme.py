#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
align syllables to morphemes in word(EoJeol)
"""
__author__ = 'krikit(krikit@naver.com)'
__copyright__ = 'Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License'


###########
# imports #
###########
import argparse
import logging
import sys

import sejong_align
import sejong_corpus


###########
# options #
###########
IS_SPOKEN = False


#############
# functions #
#############
def print_aligned(fout, pairs):
  """
  print aligned (surface, morpheme) pairs
  @param  fout   output file
  @param  pairs  (surface, morpheme) pairs
  """
  for surface, morphs in pairs:
    if isinstance(surface, unicode):
      surface = surface.encode('UTF-8')
    morphs_str = ' + '.join([str(morph) for morph in morphs])
    print >> fout, '%s\t%s' % (surface, morphs_str)


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
    sent_pairs = []
    for surface in sent.words:
      try:
        word_pairs = sejong_align.align(surface)
      except sejong_align.AlignError:
        sent_pairs = []
        break
      else:
        sent_pairs.extend(word_pairs)
    if sent_pairs:
      print_aligned(fout, sent_pairs)


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
