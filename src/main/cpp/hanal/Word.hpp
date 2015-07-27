/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_WORD_HPP
#define HANAL_WORD_HPP


//////////////
// includes //
//////////////
#include <memory>
#include <string>
#include <vector>

#include "hanal/macro.hpp"


namespace hanal {


class Char;
class Morph;
class MorphDic;
class ViterbiTrellis;


/**
 * Korean word (aka EoJeol). It's not like English words.
 * see: http://nlp.stanford.edu/fsnlp/korean.html
 */
class Word {
 public:
  SHDPTRVEC(Char) chars;    ///< vector of Korean characters consisting word
  int char_idx = -1;    ///< start position index (Korean character index in sentence except white spaces)

  explicit Word(SHDPTR(Char) char_, int char_idx_);    ///< ctor

  /**
   * @brief        tokenize UTF-8 text into words
   * @param  text  input text
   * @return       vector of words
   */
  static SHDPTRVEC(Word) tokenize(const char* text);

  /**
   * @brief         length of characters in all words
   * @param  words  vector of words
   * @return        length of characters
   */
  static int char_len(const SHDPTRVEC(Word)& words);

  /**
   * @brief   convert to wide string text
   * @return  wide string text
   */
  std::wstring to_wstr();

  /**
   * @brief   convert to reversed wide string text
   * @return  wide string text
   */
  std::wstring to_wstr_reversed();

  /**
   * @brief               forward(left to right) analyze word and add nodes to trellis
   * @param  morph_dic    morpheme dictionary
   * @param  trellis      Viterbi trellis
   * @param  trellis_idx  trellis index to add analyzed results
   */
  void analyze_forward(MorphDic* morph_dic, ViterbiTrellis* trellis, int trellis_idx);

  /**
   * @brief               backward(right to left) analyze word and add nodes to trellis
   * @param  morph_dic    morpheme dictionary
   * @param  trellis      Viterbi trellis
   * @param  trellis_idx  trellis index to add analyzed results
   */
  void analyze_backward(MorphDic* morph_dic, ViterbiTrellis* trellis, int trellis_idx);

  /**
   * @brief        merge two words into single word
   * @param  that  that word
   * @return       merged (new) word
   */
  Word operator+(const Word& that);
};


}    // namespace hanal


#endif  // HANAL_WORD_HPP
