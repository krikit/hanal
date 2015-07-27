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
  Morph(const wchar_t* lex_, SejongTag tag_);    ///< ctor

  const wchar_t* lex = nullptr;    ///< lexical form
  SejongTag tag = SejongTag::_SIZE;    ///< part-of-speech tag

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
};


}    // namespace hanal


#endif  // HANAL_MORPH_HPP
