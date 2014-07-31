#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""make training format for CRFSuite"""
__author__ = 'krikit'
__email__ = 'krikit@naver.com'
__date__ = 'creation: 2014-07-31, modification: 2014-7-31'
__copyright__ = 'nobody. feel free to use, copy and modify'


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
def print_aligned(fout, sent_pairs):
  """
  print aligned (surface, morpheme) pairs
  @param  fout        output file
  @param  sent_pairs  list of (surface, morpheme) pairs list
  """
  for word_idx, pairs in enumerate(sent_pairs):
    prev_word = sent_pairs[word_idx-1] if word_idx > 0 else None
    next_word = sent_pairs[word_idx+1] if word_idx < len(sent_pairs)-1 else None
    for pair_idx, (surface, morphs) in enumerate(pairs):
      if pair_idx > 0:
        prev_pair = pairs[pair_idx-1]
      else:
        prev_pair = prev_word[-1] if prev_word else None
      if pair_idx < len(pairs)-1:
        next_pair = pairs[pair_idx+1]
      else:
        next_pair = next_word[0] if next_word else None
      if isinstance(surface, unicode):
        surface = surface.encode('UTF-8')
      for morph_idx, morph in enumerate(morphs):
        if morph_idx > 0:
          prev_morph = morphs[morph_idx-1]
        else:
          prev_morph = prev_pair[1][-1] if prev_pair else None
        if morph_idx < len(morphs)-1:
          next_morph = morphs[morph_idx+1]
        else:
          next_morph = next_pair[1][0] if next_pair else None
        features = []
        features.append('L_0=%s' % morph.lex)
        features.append('S_0=%s' % surface)
        if prev_morph:
          features.append('L-1=%s' % prev_morph.lex)
        else:
          features.append('BOS')
        if next_morph:
          features.append('L+1=%s' % next_morph.lex)
        else:
          features.append('EOS')
        if prev_pair:
          prev_surface = prev_pair[0]
          if isinstance(prev_surface, unicode):
            prev_surface = prev_surface.encode('UTF-8')
          features.append('S-1=%s' % prev_surface)
        if next_pair:
          next_surface = next_pair[0]
          if isinstance(next_surface, unicode):
            next_surface = next_surface.encode('UTF-8')
          features.append('S+1=%s' % next_surface)
        if pair_idx == 0 and morph_idx == 0:
          features.append('LSP')
        if pair_idx == len(pairs)-1 and morph_idx == len(morphs)-1:
          features.append('RSP')
        print >> fout, '%s\t%s' % (morph.tag, ' '.join(features))
  print >> fout


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
    for word in sent.words:
      try:
        word_pairs = sejong_align.align(word)
      except sejong_align.AlignError:
        sent_pairs = []
        break
      else:
        sent_pairs.append(word_pairs)
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
