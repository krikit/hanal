/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_MORPH_HPP
#define HANAL_MORPH_HPP


//////////////
// includes //
//////////////
#include <memory>
#include <string>
#include <vector>

#include "hanal/macro.hpp"
#include "hanal/SejongTag.hpp"


namespace hanal {


/**
 * morpheme with lexical form and it's part-of-speech tag
 */
class Morph {
 public:
  SejongTag tag = SejongTag::_SIZE;    ///< part-of-speech tag

  explicit Morph(const wchar_t* lex, SejongTag tag_);    ///< ctor
  explicit Morph(std::unique_ptr<wchar_t[]>&& lex, SejongTag tag_);    ///< ctor, NOLINT

  const wchar_t* lex();    ///< get lexical form

  /**
   * @brief             parse morpheme
   * @param  morph_str  morpheme string to parse
   * @return            morpheme object (shared pointer)
   */
  static SHDPTR(Morph) parse(wchar_t* morph_str);

  /**
   * @biref              parse list of morphemes
   * @param  morphs_str  morpheme list string to parse
   * @return             vector of morphemes
   */
  static SHDPTRVEC(Morph) parse_anal_result(wchar_t* morphs_str);

  /**
   * @biref                   parse analysis results (list of analysis results)
   * @param  anal_resuls_str  analysis results string to parse
   * @return                  vector of analysis results
   */
  static std::vector<SHDPTRVEC(Morph)> parse_anal_result_vec(wchar_t* anal_results_str);

  std::string str();    ///< get string for debugging

  /**
   * @brief               get string of vector of morphems for debugging
   * @param  anal_result  analysis result (vector of morphems)
   * @return              string
   */
  static std::string str(const SHDPTRVEC(Morph)& anal_result);

 private:
  const wchar_t* _lex = nullptr;    ///< lexical form from morpheme dic
  std::unique_ptr<wchar_t[]> _lex_cpy;    ///< copied lexical form for estimated morpheme
};


}    // namespace hanal


#endif  // HANAL_MORPH_HPP
