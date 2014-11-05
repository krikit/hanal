#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""make training format for CRFsuite"""
__author__ = 'krikit'
__email__ = 'krikit@naver.com'
__date__ = 'creation: 2014-07-31, modification: 2014-11-04'
__copyright__ = 'nobody. feel free to use, copy and modify'


###########
# imports #
###########
import argparse
import logging
import random

import sejong_align
import sejong_corpus


#############
# constants #
#############
_MAX_DELTA = 10000    # delta = (max size) - (min size) when splitting output


###########
# options #
###########
IS_SPOKEN = False


#############
# functions #
#############
def select_file(fouts, sizes, new_size):
  """
  select file to print out
  if delta is small, randomly select. or just select min sized file
  @param  fouts     output file list
  @param  sizes     size of file list
  @param  new_size  size of new output
  @return           index of list
  """
  if len(fouts) == 1:
    sizes[0] += new_size
    return fouts[0]
  idx_size_list = [(idx, size) for idx, size in enumerate(sizes)]
  idx_size_list.sort(key=lambda idx_size: idx_size[1])
  delta = idx_size_list[-1][1] - idx_size_list[0][1]
  if delta < _MAX_DELTA:
    idx = random.randint(0, len(fouts)-1)
  else:
    idx = idx_size_list[0][0]
  sizes[idx] += new_size
  return fouts[idx]


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
        features.append('L_0=%s' % morph.lex)    # current morpheme
        features.append('S_0=%s' % surface)    # current surface
        if prev_morph:
          features.append('L-1=%s' % prev_morph.lex)    # previous morpheme
        else:
          features.append('BOS')   # begin of sentence
        if next_morph:
          features.append('L+1=%s' % next_morph.lex)    # next morpheme
        else:
          features.append('EOS')    # end of sentence
        if prev_pair:
          prev_surface = prev_pair[0]
          if isinstance(prev_surface, unicode):
            prev_surface = prev_surface.encode('UTF-8')
          features.append('S-1=%s' % prev_surface)    # previous surface
        if next_pair:
          next_surface = next_pair[0]
          if isinstance(next_surface, unicode):
            next_surface = next_surface.encode('UTF-8')
          features.append('S+1=%s' % next_surface)    # next surface
        if pair_idx == 0 and morph_idx == 0:
          features.append('LSP')    # left space (space at left)
        if pair_idx == len(pairs)-1 and morph_idx == len(morphs)-1:
          features.append('RSP')    # right space (space at right)
        print >> fout, '%s\t%s' % (morph.tag, '\t'.join(features))
  print >> fout


########
# main #
########
def main(fin_names, fout_name, split_num):
  """
  make training format for CRFsuite
  @param  fin_names   list of input files
  @param  fout_name   output file path
  @param  split_num   split number of output file
  """
  fouts = []
  sizes = []
  if split_num > 1:
    fouts = [open('%s.%d' % (fout_name, idx), 'w') for idx in range(split_num)]
    sizes = [0] * split_num
  else:
    fouts = [open(fout_name, 'w')]
    sizes = [0]

  for sent in sejong_corpus.load(IS_SPOKEN, fin_names):
    if not sent.is_good_tags():
      logging.error('Invalid tag in sentence: %s', sent)
      continue
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
      surfaces = [surface.encode('UTF-8') for pairs in sent_pairs for surface, _ in pairs]
      new_size = sum([len(surface) for surface in surfaces])
      print_aligned(select_file(fouts, sizes, new_size), sent_pairs)


if __name__ == '__main__':
  _PARSER = argparse.ArgumentParser(description='make training format for CRFsuite')
  _PARSER.add_argument('input', help='input files', metavar='FILE', nargs='+')
  _PARSER.add_argument('-o', '--output', help='output file', metavar='FILE', required=True)
  _PARSER.add_argument('--split-num', help='split number of output file <default: 1>', metavar='NUM',
      type=int, default=1)
  _PARSER.add_argument('--seed', help='random seed number', metavar='NUM', type=int)
  _PARSER.add_argument('--is-spoken', help='whether spoken corpus or not', action='store_true')
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
  if _ARGS.seed:
    random.seed(_ARGS.seed)
  main(_ARGS.input, _ARGS.output, _ARGS.split_num)
