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


namespace hanal {


class Char;


/**
 * Korean word (aka EoJeol). It's not like English words.
 * see: http://nlp.stanford.edu/fsnlp/korean.html
 */
class Word {
 public:
  std::vector<std::shared_ptr<Char>> chars;    ///< vector of Korean characters consisting word

  explicit Word(std::shared_ptr<Char> char_);    ///< ctor

  /**
   * @brief        tokenize UTF-8 text into words
   * @param  text  input text
   * @return       vector of words
   */
  static std::vector<std::shared_ptr<Word>> tokenize(const char* text);
};


}    // namespace hanal


#endif  // HANAL_WORD_HPP
