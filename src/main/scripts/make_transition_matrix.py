#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
make transition matrix
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


#############
# functions #
#############
def load_transition_matrix(fin, label_dic):
  """
  load transtion matrix from dumped model file
  :param  fin:        input file
  :param  label_dic:
  :return:
  """
  trans_matrix = [[0.0] * len(label_dic) for _ in range(len(label_dic))]
  for line in fin:
    line = line.strip()
    if line == '}':
      break
    _, from_label, _, to_label, val = line.split()
    from_id = label_dic[from_label]
    to_id = label_dic[to_label[:-1]]    # remove trailing ':'
    trans_matrix[to_id][from_id] = float(val)
  return trans_matrix


########
# main #
########
def main(fin, fout):
  """
  make transition matrix
  :param  fin:   input file
  :param  fout:  output file
  """
  label_dic = {label: idx for idx, label in enumerate(sorted(list(sejong_corpus.TAG_SET)))}

  trans_matrix = []
  for line in fin:
    if line.startswith('TRANSITIONS = {'):
      trans_matrix = load_transition_matrix(fin, label_dic)
      break

  reversed_label_dic = {idx: label for label, idx in label_dic.items()}
  for to_id in range(0, len(label_dic)):
    for from_id in range(0, len(label_dic)):
      fout.write(struct.Struct('f').pack(trans_matrix[to_id][from_id]))
      logging.debug('%s --> %s: %f', reversed_label_dic[from_id], reversed_label_dic[to_id],
                    trans_matrix[to_id][from_id])


if __name__ == '__main__':
  _PARSER = argparse.ArgumentParser(description='make syllable-morpheme TRIE dictionary')
  _PARSER.add_argument('--input', help='input file <default: stdin>', metavar='FILE', type=file, default=sys.stdin)
  _PARSER.add_argument('--output', help='output stem <defulat: stdout>', metavar='FILE',
                       type=argparse.FileType('w'), default=sys.stdout)
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
