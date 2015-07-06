#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
make state-features dictionary
"""
__author__ = 'krikit(krikit@naver.com)'
__copyright__ = 'Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License'


###########
# imports #
###########
import argparse
import logging
import struct
import sys

import sejong_corpus
import trie


#############
# constants #
#############
# string tag to num index map
_SEJONG_TAG_TO_IDX = {tag: idx for idx, tag in enumerate(sorted(list(sejong_corpus.TAG_SET)))}


#############
# functions #
#############
def load_state_feat_dic(fin):
  """
  load state-features dictionary from crfsuite model file
  :param  fin:  input file
  :return:      state-features dictionary
  """
  state_feat_dic = {}
  is_in_state_feat = False
  for line_num, line in enumerate(fin, start=1):
    if line_num % 1000000 == 0:
      logging.info('%dm-th line reading..', line_num / 1000000)
    line = line.strip()
    if is_in_state_feat:
      if line == '}':
        break
      try:
        state, feat, weight = parse_state_feat_line(line)
        key = chr(ord('A') + _SEJONG_TAG_TO_IDX[state]) + feat
        state_feat_dic[unicode(key, 'UTF-8')] = weight
      except RuntimeError, run_err:
        logging.error('%s at line(%d): %s', run_err, line_num, line)
    elif line.startswith('STATE_FEATURES = {'):
      is_in_state_feat = True
  return state_feat_dic


def parse_state_feat_line(line):
  """
  parse single state-feature line
  :param  line:  line
  :return:       (state, feature, weight) tuple. throws RuntimeError when parse failed
  """
  cols = line.split()
  if len(cols) != 5:
    raise RuntimeError('Invalid column number:%d' % len(cols))
  state = cols[3]
  if not state.endswith(':'):
    raise RuntimeError('Colon not found at the end of state')
  return state[:-1], cols[1], float(cols[4])


def build_trie(state_feat_dic):
  """
  build trie nodes with state-features dictionary
  :param  state_feat_dic:  state-features dictionary
  :return:                 root node of trie
  """
  trie_root = trie.Node()
  for num, state_feat in enumerate(sorted(state_feat_dic.keys()), start=1):
    if num % 1000000 == 0:
      logging.info('%dm-th trie node inserting..', num / 1000000)
    trie_root.insert(state_feat, state_feat_dic[state_feat])
  return trie_root


def write_to_file(trie_root, output_stem):
  """
  write trie to file
  :param  trie_root:    root node of trie
  :param  output_stem:  output file stem
  """
  fout_key = open('%s.trie' % output_stem, 'wb')
  fout_val = open('%s.val' % output_stem, 'w')
  val_serial = 0
  nodes = trie_root.breadth_first_traverse()
  for idx, node in enumerate(nodes):
    if (idx+1) % 1000000 == 0:
      logging.info('%dm-th node writting..', (idx+1) / 1000000)
    logging.debug(u'%d:%s', idx, node)
    val_idx = -1
    if node.value:
      val_idx = val_serial
      val_serial += 1
      fout_val.write(struct.Struct('f').pack(node.value))
    fout_key.write(node.pack(val_idx))
  logging.info('Number of nodes: %d', len(nodes))
  logging.info('Number of values: %d', val_serial)


########
# main #
########
def main(fin, output_stem):
  """
  make state-features dictionary
  :param  fin:          input file
  :param  output_stem:  output file name without extension
  """
  state_feat_dic = load_state_feat_dic(fin)
  trie_root = build_trie(state_feat_dic)
  write_to_file(trie_root, output_stem)


if __name__ == '__main__':
  _PARSER = argparse.ArgumentParser(description='make state-features dictionary')
  _PARSER.add_argument('--input', help='input file <default: stdin>', metavar='FILE', type=file, default=sys.stdin)
  _PARSER.add_argument('-o', '--output', help='output stem', metavar='FILE STEM', required=True)
  _PARSER.add_argument('--log-level', help='set logging level', metavar='LEVEL')
  _PARSER.add_argument('--log-file', help='set log file <default: stderr>', metavar='FILE')
  _ARGS = _PARSER.parse_args()
  _LOG_CFG = {'format':'[%(asctime)-15s] %(levelname)-8s %(message)s', 'datefmt':'%Y-%m-%d %H:%M:%S'}
  if _ARGS.log_level:
    _LOG_CFG['level'] = eval('logging.%s' % _ARGS.log_level.upper())    # pylint: disable=W0123
  if _ARGS.log_file:
    _LOG_CFG['filename'] = _ARGS.log_file
  logging.basicConfig(**_LOG_CFG)    # pylint: disable=W0142
  main(_ARGS.input, _ARGS.output)
