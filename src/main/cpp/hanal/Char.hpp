/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_CHAR_HPP
#define HANAL_CHAR_HPP


//////////////
// includes //
//////////////
#include <memory>
#include <string>
#include <vector>


namespace hanal {


/**
 * Korean character (aka EumJeol)
 * see: http://nlp.stanford.edu/fsnlp/korean.html
 */
class Char {
 public:
  const std::wstring SPACE = L" \t\v\r\n\u3000";    ///< space characters

  wchar_t wchar = L'\0';    ///< converted wide character
  const char* start = nullptr;    ///< start position (zero based, inclusive) of original UTF-8 text
  const char* end = nullptr;    ///< end position (exclusive) of original UTF-8 text

  Char(wchar_t wchar_, const char* start_, const char* end_);    ///< ctor

  /**
   * @brief        tokenize UTF-8 text into words
   * @param  text  input text
   * @return       vector of words
   */
  static std::vector<std::shared_ptr<Char>> characterize(const char* text);

  /**
   * whether is space or not
   * @return  whether is space
   */
  bool is_space();
};


}    // namespace hanal


#endif  // HANAL_CHAR_HPP


