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
from collections import namedtuple
import editdistance
import logging

# for decomposing syllables into consonants and vowels
# http://biohackers.net/wiki/PythonAndHangul#head-4788cfe894c55238b6133994c6ab356302f63ec8
import hangul
import sejong_corpus


#############
# constants #
#############
# convert last consonant to first consonant
_JONG_TO_CHO = {
  u'ㄴ': u'\u11ab',
  u'ㄹ': u'\u11af',
}
_SIMILARITY_MIN = 0.5   # word to morphemes similarity threshold based on edit distance
# correct word morphemes aligns (but low similarity)
_CORRECT_ALIGNS = set([
  '해 하/XSV + 아/EC',
  '내 나/NP + 의/JKG',
  '해 하/XSA + 아/EC',
  '게 것/NNB + 이/JKS',
  '해 하/VV + 아/EC',
  '제 저/NP + 의/JKG',
  '돼 되/XSV + 어/EC',
  '게 것/NNB + 이/JKC',
  '테 터/NNB + 이/VCP',
  '져 지/VX + 어/EC',
  '치 하/XSA + 지/EC',
  '해 하/VX + 아/EC',
  '해졌 하/XSA + 아/EC + 지/VX + 었/EP',
  '돼 되/VV + 어/EC',
  '와 오/VV + 아/EC',
  '네 너/NP + 의/JKG',
  '키 하/XSV + 기/ETN',
  '’ \'/SS',
  '거 것/NNB + 이/VCP',
  '요 이/VCP + 오/EF',
  '케 하/XSV + 게/EC',
  '봐 보/VX + 아/EC',
  '해왔 하/XSV + 아/EC + 오/VX + 았/EP',
  '치 하/XSV + 지/EC',
  '돼 되/VV + 어/EF',
  '‘ \'/SS',
  '게 것/NNB + 이/VCP',
  '해 하/VV + 아/EF',
  '“ "/SS',
  '뭘 무엇/NP + ᆯ/JKO',
  '쳐 치/VV + 어/EC',
  '봐 보/VX + 아/EF',
  '해 하/XSA + 아/EF',
  '해 하/XSV + 아/EF',
  '해 하/VX + 아/EF',
  '봐 보/VV + 아/EC',
  '케 하/XSA + 게/EC',
  '해져 하/XSA + 아/EC + 지/VX + 어/EC',
  '해왔 하/VV + 아/EC + 오/VX + 았/EP',
  '뭘 무엇/NP + 을/JKO',
  '마 말/VX + 아/EF',
  '봐 보/VV + 아/EF',
  '여 이/VCP + 어/EF',
  '” "/SS',
  '와 오/VV + 아/EF',
  '와 오/VX + 아/EC',
  '줘 주/VX + 어/EF',
  '져 지/VV + 어/EC',
  '찮 하/XSA + 지/EC + 않/VX',
  '놨 놓/VX + 았/EP',
  '돼 되/XSV + 어/EF',
  "’ '/SS + 이/VCP",
  '마 말/VX + 아/EC',
  '요 이/VCP + 오/EC',
  '어째 어찌/MAG + 하/XSV + 아/EC',
  '뭔 무엇/NP + 이/VCP + ᆫ/ETM',
  '여 이/VCP + 어/EC',
  '놔 놓/VV + 아/EC',
  '줘 주/VV + 어/EF',
  '∼ ~/SO',
  '줘 주/VV + 어/EC',
  '뭐 뭣/NP + 이/VCP',
  '꿔 꾸/VV + 어/EC',
  '쪄 찌/VV + 어/EC',
  '쏴 쏘/VV + 아/EC',
  '마 말/VV + 아/EF',
  '줘 주/VX + 어/EC',
  '랬 라/EC + 하/VV + 았/EP',
  '셔 시/EP + 어/EF',
  '랬 이/VCP + 라/EC + 하/VV + 았/EP',
  '세요 이/VCP + 시/EP + 어요/EF',
  '뭘로 무엇/NP + 으로/JKB',
  '뭘까 무엇/NP + 이/VCP + ᆯ까/EF',
  '돼 되/XSA + 어/EC',
  '쟤 저/MM + 아이/NNG',
  '둬 두/VV + 어/EC',
  '테 터/NNB + (이)/VCP',
  '해야 하/VV + ㅕ야/EC + (하)/VX',
  '놔 놓/VV + ㅏ/EC',
])


#########
# types #
#########
# word string (unicode) and morpheme list pair
WordStrMorphsPair = namedtuple('WordStrMorphsPair', ['word_str', 'morphs'])

class AlignError(Exception):
  """
  alignment exception
  """
  pass


#############
# functions #
#############
def decompose(uni_word):
  """
  decompose syllables into consonants and vowels
  :param    uni_word:  word
  :return:             decomposed word
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
  :param    text:  text
  :return:         hex coded string
  """
  return ' '.join([hex(ord(char)) for char in unicode(text, 'UTF-8')])


def match_pfx(uni_word, morphs):
  """
  try to match with word prefix and head of morpheme list
  :param    uni_word:  unicode word
  :param    morphs:    morpheme list
  :return:             (match character length in word, match morpheme number in morpheme list) tuple
  """
  uni_morph = unicode(morphs[0].lex, 'UTF-8')
  if uni_word.startswith(uni_morph):    # just one morpheme starts with word
    return len(uni_morph), 1
  if len(morphs) == 1:    # only one morpheme is left
    morph_dec = decompose(morphs[0].lex)
    word_dec = decompose(uni_word)
    if word_dec == morph_dec:
      return 1, len(uni_word)
    else:
      return -1, -1
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
  :param    uni_word:  unicode word
  :param    morphs     morpheme list
  :return:             (match character length in word, match morpheme number in morpheme list) tuple
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


def log_not_aligned(word, forward_pairs, sandwich_word, sandwich_morphs, backward_pairs):
  """
  logging not aligned word
  :param  word:             word
  :param  forward_pairs:    forward pairs
  :param  sandwich_word:    sandwich word
  :param  sandwich_morphs:  sandwich morphemes
  :param  backward_pairs:   backward pairs
  """
  if isinstance(word, unicode):
    word = word.encode('UTF-8')
  if isinstance(sandwich_word, unicode):
    sandwich_word = sandwich_word.encode('UTF-8')
  logging.error('Not aligned word: [%s]', word)
  for word_str, morphs in forward_pairs:
    if isinstance(word_str, unicode):
      word_str = word_str.encode('UTF-8')
    morph_str = ' + '.join([str(morph) for morph in morphs])
    logging.error('  Forward:   [%s]\t%s', word_str, morph_str)
  if sandwich_word and sandwich_morphs:
    logging.error('  Sandwich:  [%s]\t%s', sandwich_word, ' + '.join([str(morph) for morph in sandwich_morphs]))
  elif sandwich_word:
    logging.error('  SW Word:   [%s]', sandwich_word)
  else:
    logging.error('  SW Morphs: %s', ' + '.join([str(morph) for morph in sandwich_morphs]))
  for word_str, morphs in backward_pairs:
    if isinstance(word_str, unicode):
      word_str = word_str.encode('UTF-8')
    morph_str = ' + '.join([str(morph) for morph in morphs])
    logging.error('  Backward:  [%s]\t%s', word_str, morph_str)


def forward_search(forward_pairs, uni_word, morphs):
  """
  search from the beginning
  :param    forward_pairs:  pair list to put
  :param    uni_word:       word
  :param    morphs:         morpheme list
  :return:                  remaining (uni_word, morphs) pair
  """
  while uni_word and morphs:
    word_match_len, morph_match_num = match_pfx(uni_word, morphs)
    if word_match_len == -1:
      break
    forward_pairs.append(WordStrMorphsPair(uni_word[:word_match_len], morphs[:morph_match_num]))
    uni_word = uni_word[word_match_len:]
    morphs = morphs[morph_match_num:]
  return uni_word, morphs


def backward_search(backward_pairs, uni_word, morphs):
  """
  search from the end
  :param    backward_pairs:  pair list to put
  :param    uni_word:        word
  :param    morphs:          morpheme list
  :return:                   remaining (uni_word, morphs) pair
  """
  while uni_word and morphs:
    word_match_len, morph_match_num = match_sfx(uni_word, morphs)
    if word_match_len == -1:
      break
    backward_pairs.insert(0, WordStrMorphsPair(uni_word[-word_match_len:], morphs[-morph_match_num:]))
    uni_word = uni_word[:-word_match_len]
    morphs = morphs[:-morph_match_num]
  return uni_word, morphs


def calc_similarity(lhs, rhs):
  """
  calculate similarity based on edit distance
  :param    lhs:  left hand side
  :param    rhs:  right hand side
  :return:        similarity
  """
  lhs_decomp = decompose(lhs)
  rhs_decomp = decompose(rhs)
  dist = editdistance.eval(lhs_decomp, rhs_decomp)
  max_len = max(len(lhs_decomp), len(rhs_decomp))
  sim = float(max_len - dist) / float(max_len)
  logging.debug('SIM: [%s] vs [%s] ==> %d / %d = %f', lhs.encode('UTF-8'), rhs.encode('UTF-8'),
      max_len - dist, max_len, sim)
  return sim


def proc_sw_both_not_empty(forward_pairs, uni_word, morphs, backward_pairs):
  """
  process sandwich(remaining) (word, morphs) pair when both word and morpheme list are not empty
  :param    forward_pairs:   forward pair list
  :param    uni_word:        word
  :param    morphs:          morpheme list
  :param    backward_pairs:  backward pair list
  :return:                   sandwich pair list. AlignError raised when error occurs
  """
  sandwich_pairs = []
  morphs_str = ' + '.join([str(morph) for morph in morphs])
  word_morphs_str = '%s %s' % (uni_word.encode('UTF-8'), morphs_str)
  if word_morphs_str in _CORRECT_ALIGNS:
    sandwich_pairs.append(WordStrMorphsPair(uni_word, morphs))
  elif word_morphs_str in ['해야 하/VV + 아야/EC + 하/VX', '해야 하/XSV + 아야/EC + 하/VX', '야 아야/EC + 하/VX',
      '야 어야/EC + 하/VX', '봐야 보/VX + 아야/EC + 하/VX', '해야 하/VX + 아야/EC + 하/VX'] \
      and backward_pairs and str(backward_pairs[0].morphs[0]) == '겠/EP':
    sandwich_pairs.append(WordStrMorphsPair(uni_word, morphs[:-1]))
    backward_pairs[0].morphs.insert(0, morphs[-1])
    del morphs[:]
  elif word_morphs_str in ['졌 어/EC + 지/VX + 었/EP', '져 어/EC + 지/VX + 어/EC', '봐야 아/EC + 보/VX + 아야/EC',
      '왔 아/EC + 오/VX + 았/EP', '왔 어/EC + 오/VX + 았/EP', '봤 아/EC + 보/VX + 았/EP', '봐 아/EC + 보/VX + 아/EF',
      '왔 아/EC + 오/VV + 았/EP', '냈 아/EC + 내/VX + 었/EP', '내 아/EC + 내/VX + 어/EC'] \
      and forward_pairs and forward_pairs[-1].morphs[-1].tag == 'VV':
    forward_pairs[-1].morphs.append(morphs[0])
    sandwich_pairs.append(WordStrMorphsPair(uni_word, morphs[1:]))
    del morphs[:]
  elif word_morphs_str == '봐야 아/EC + 보/VX + 아야/EC + 하/VX' \
      and forward_pairs and forward_pairs[-1].morphs[-1].tag == 'VV' \
      and backward_pairs and str(backward_pairs[0].morphs[0]) == '겠/EP':
    forward_pairs[-1].morphs.append(morphs[0])
    sandwich_pairs.append(WordStrMorphsPair(uni_word, morphs[1:-1]))
    backward_pairs[0].morphs.insert(0, morphs[-1])
    del morphs[:]
  elif uni_word in [u'해졌', u'해봤', u'해했', u'해봐', u'해줬', u'해줘', u'해와', u'해져'] and len(morphs) == 4:
    sandwich_pairs.append(WordStrMorphsPair(uni_word[0], morphs[:2]))
    sandwich_pairs.append(WordStrMorphsPair(uni_word[1], morphs[2:]))
    del morphs[:]
  elif word_morphs_str == '잖 이/VCP + 지/EC + 않/VX' \
      and forward_pairs and forward_pairs[-1].morphs[-1].tag.startswith('N'):
    forward_pairs[-1].morphs.append(morphs[0])
    sandwich_pairs.append(WordStrMorphsPair(uni_word, morphs[1:]))
    del morphs[:]
  elif word_morphs_str in ['해서 하/VV + 아서/EC + 이/VCP', '해서 하/XSV + 아서/EC + 이/VCP',
      '해서 하/VX + 아서/EC + 이/VCP', '서] 아서/EC+이/VCP'] \
      and backward_pairs and backward_pairs[0].morphs[0].tag.startswith('E'):
    sandwich_pairs.append(WordStrMorphsPair(uni_word, morphs[:-1]))
    backward_pairs[0].morphs.insert(0, morphs[-1])
    del morphs[:]
  elif word_morphs_str in ['서 아서/EC + 이/VCP'] \
      and backward_pairs and backward_pairs[0].morphs[0].tag.startswith('E'):
    sandwich_pairs.append(WordStrMorphsPair(uni_word, morphs[:-1]))
    backward_pairs[0].morphs.insert(0, morphs[-1])
    del morphs[:]
  else:
    sim = calc_similarity(uni_word, unicode(''.join([morph.lex for morph in morphs]), 'UTF-8'))
    if sim >= _SIMILARITY_MIN:
      # string similarity is greater than threshold
      sandwich_pairs.append(WordStrMorphsPair(uni_word, morphs))
    else:
      raise AlignError()
  return sandwich_pairs


def proc_sw_only_word(word, forward_pairs, uni_word, backward_pairs):
  """
  process sandwich(remaining) (word, morphs) pair when word is empty and only single morpheme is left
  :param    word:            sejong.Word object
  :param    forward_pairs:   forward pair list
  :param    uni_word:        word
  :param    backward_pairs:  backward pair list
  :return:                   sandwich pair list. AlignError raised when error occurs
  """
  sandwich_pairs = []
  if uni_word == u'.' and word.raw.endswith('.'):
    sandwich_pairs.append(WordStrMorphsPair(uni_word, [sejong_corpus.Morph.parse('./SF'),]))
  elif uni_word == u'~' and (not backward_pairs or len(backward_pairs) == 1):
    sandwich_pairs.append(WordStrMorphsPair(uni_word, [sejong_corpus.Morph.parse('~/SO'),]))
  elif uni_word == u',' and not backward_pairs:
    sandwich_pairs.append(WordStrMorphsPair(uni_word, [sejong_corpus.Morph.parse('~/SP'),]))
  elif uni_word == u'<pause/>':
    pass
  else:
    raise AlignError()
  return sandwich_pairs


def proc_sw_only_morphs(forward_pairs, morphs, backward_pairs):
  """
  process sandwich(remaining) (word, morphs) pair when word is empty and only single morpheme is left
  :param    forward_pairs:   forward pair list
  :param    morphs:          morpheme list
  :param    backward_pairs:  backward pair list
  :return:                   sandwich pair list. AlignError raised when error occurs
  """
  sandwich_pairs = []
  if not backward_pairs:
    forward_pairs[-1].morphs.extend(morphs)
  elif len(morphs) == 1:
    morph = morphs[0]
    morph_str = str(morph)
    if morph_str in ['이/VCP', '하/VX'] and backward_pairs[0].morphs[0].tag.startswith('E'):
      # '이' 긍정지정사나 '하' 보조용언 뒤에 어미가 나올 경우
      backward_pairs[0].morphs.insert(0, morphs[0])
      del morphs[:]
    elif morph_str == '에/JKB' and backward_pairs[0].morphs[0].tag == 'JX':
      # '에' 부사격조사 뒤에 보조사가 나올 경우
      backward_pairs[0].morphs.insert(0, morphs[0])
      del morphs[:]
    elif morph_str == 'ᆯ/ETM' and forward_pairs[-1].morphs[-1].tag.startswith('V'):
      # 'ㄹ' 관형형전성어미 앞에 용언이 나올 경우
      forward_pairs[-1].morphs.append(morphs[0])
      del morphs[:]
    elif morph.tag in ['EC', 'EF'] and forward_pairs[-1].morphs[-1].tag.startswith('V'):
      # 연결어미나 종결어미 앞에 용언이 나올 경우
      forward_pairs[-1].morphs.append(morphs[0])
      del morphs[:]
    elif morph.tag.startswith('XS'):
      # append suffixes to the end of forward pair list
      forward_pairs[-1].morphs.append(morphs[0])
      del morphs[:]
    else:
      raise AlignError()
  else:
    morphs_str = ' + '.join([str(morph) for morph in morphs])
    if morphs_str == '(/SS + 대북/NNG + (/SS + 대북/NNG + )/SS + )/SS' and forward_pairs[-1].word_str == u'대북':
      del morphs[:]
    elif morphs_str == '(/SS + 동경/NNP + )/SS' and forward_pairs[-1].word_str == u'도쿄':
      del morphs[:]
    else:
      raise AlignError()
  return sandwich_pairs


def proc_sandwich(word, forward_pairs, uni_word, morphs, backward_pairs):
  """
  process sandwich(remaining) (word, morphs) pair
  :param    word:            sejong.Word object
  :param    forward_pairs:   forward pair list
  :param    uni_word:        word
  :param    morphs:          morpheme list
  :param    backward_pairs:  backward pair list
  :return:                   sandwich pair list. AlignError raised when error occurs
  """
  sandwich_pairs = []
  if uni_word and morphs:
    sandwich_pairs = proc_sw_both_not_empty(forward_pairs, uni_word, morphs, backward_pairs)
  elif uni_word:
    sandwich_pairs = proc_sw_only_word(word, forward_pairs, uni_word, backward_pairs)
  elif morphs:
    sandwich_pairs = proc_sw_only_morphs(forward_pairs, morphs, backward_pairs)
  return sandwich_pairs


def align(word):
  """
  align syllables to morphemes in word(EoJeol)
  :param    word:  sejong.Word object
  :return:         WordStrMorphsPair list. AlignError raised when error occurs
  """
  uni_word = unicode(word.raw, 'UTF-8')
  morphs = word.morphs
  forward_pairs = []
  uni_word, morphs = forward_search(forward_pairs, uni_word, morphs)
  backward_pairs = []
  uni_word, morphs = backward_search(backward_pairs, uni_word, morphs)
  try:
    sandwich_pairs = proc_sandwich(word, forward_pairs, uni_word, morphs, backward_pairs)
  except AlignError, align_err:
    log_not_aligned(word.raw, forward_pairs, uni_word, morphs, backward_pairs)
    raise align_err
  return forward_pairs + sandwich_pairs + backward_pairs
