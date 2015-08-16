/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_UTIL_HPP
#define HANAL_UTIL_HPP


//////////////
// includes //
//////////////
#include <locale>
#include <map>
#include <string>

#include "hanal/SejongTag.hpp"


namespace hanal {


class Util {
 public:
  /**
   * @brief        wide string to UTF-8 string
   * @param  wstr  wide string
   * @return       UTF-8 string
   */
  static std::string to_utf8(const wchar_t* wstr);

  /**
   * @brief        wide string to UTF-8 string
   * @param  wstr  wide string
   * @return       UTF-8 string
   */
  static std::string to_utf8(const std::wstring& wstr);

  /**
   * @brief       convert from string part-of-speech tag to SejongTag
   * @param  tag  part-of-speech tag
   * @return      SejongTag
   */
  static SejongTag to_sejong(const wchar_t* tag);

  /**
   * @brief          convert SejongTag to string part-of-speech tag
   * @param  sejong  SejongTag
   * @return         part-of-speech tag
   */
  static const wchar_t* from_sejong(SejongTag sejong);

 private:
  static std::map<std::wstring, SejongTag> _to_sejong_map;    ///< string tag to int tag map
  static std::array<const wchar_t*, static_cast<size_t>(SejongTag::_SIZE)> _from_sejong_arr;    ///< string tags
};


}    // namespace hanal


#endif  // HANAL_UTIL_HPP
