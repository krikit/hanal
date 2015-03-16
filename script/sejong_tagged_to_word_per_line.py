#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
make word(EoJeol) per line formatted corpus from Sejong tagged corpus
"""
__author__ = 'krikit(krikit@naver.com)'
__copyright__ = 'Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License'


###########
# imports #
###########
import argparse
import logging
import sys

import sejong_corpus


###########
# options #
###########
IS_SPOKEN = False


########
# main #
########
def main(fin_names, fout):
  """
  make word(EoJeol) per line formatted corpus from Sejong tagged corpus
  :param  fin_names:  list of input files
  :param  fout:       output file
  """
  for sent in sejong_corpus.load(IS_SPOKEN, fin_names):
    print >> fout, sent
    print >> fout


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
