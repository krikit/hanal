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

#include "hanal/macro.hpp"


namespace hanal {


/**
 * Korean character (aka EumJeol)
 * see: http://nlp.stanford.edu/fsnlp/korean.html
 */
class Char {
 public:
  enum class Type : int {    ///< character types for unknown word estimation
    UNK = 0,    ///< unknown (not identified)
    SPACE,    ///< white spaces
    HANGUL,    ///< Hangul syllables
    LATIN,    ///< (SL) Latin characters
    NUMBER,    ///< (SN) numbers
    CJK,    ///< (SH) CJK characters
    FOREIGN,    ///< (SL) foreign letters except Latin
    ELLIPSIS,    ///< (SE) ellipsis
    PERIOD,    ///< (SF) period, question/exclamation mark
    O_MARK,    ///< (SO) O mark, tilde
    COMMA,    ///< (SP) comma, dot, colon, slash
    QUOTE,    ///< (SS) quote, parenthesis, dash
    SYMBOL    ///< other symbols (SW)
  };

  static const std::wstring SPACE;    ///< space characters

  wchar_t wchar = L'\0';    ///< converted wide character
  const char* start = nullptr;    ///< start position (zero based, inclusive) of original UTF-8 text
  const char* end = nullptr;    ///< end position (exclusive) of original UTF-8 text
  Type type = Type::UNK;    ///< character type

  Char(wchar_t wchar_, const char* start_, const char* end_);    ///< ctor

  /**
   * @brief        tokenize UTF-8 text into words
   * @param  text  input text
   * @return       vector of words
   */
  static SHDPTRVEC(Char) characterize(const char* text);

  bool is_space();    ///< whether is space or not

  static bool is_space(wchar_t wchar);    ///< whether is space or not
  static bool is_hangul(wchar_t wchar);    ///< whether is Hangul or not
  static bool is_latin(wchar_t wchar);    ///< whether is Latin character or not
  static bool is_number(wchar_t wchar);    ///< whether is number or not
  static bool is_cjk(wchar_t wchar);    ///< whether is CJK character or not
  static bool is_foreign(wchar_t wchar);    ///< whether is foreign letters or not
  static bool is_ellipsis(wchar_t wchar);    ///< whether is ellipsis or not
  static bool is_period(wchar_t wchar);    ///< whether is period or not
  static bool is_o_mark(wchar_t wchar);    ///< whether is O mark or not
  static bool is_comma(wchar_t wchar);    ///< whether is comma or not
  static bool is_quote(wchar_t wchar);    ///< whether is quote or not
  static bool is_symbol(wchar_t wchar);    ///< whether is symbol or not
};


}    // namespace hanal


#endif  // HANAL_CHAR_HPP


