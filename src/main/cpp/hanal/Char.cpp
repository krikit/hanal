/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Char.hpp"


//////////////
// includes //
//////////////
#include <locale>

#include "boost/lexical_cast.hpp"
#include "hanal/Except.hpp"


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
Char::Char(wchar_t wchar_, const char* start_, const char* end_)
    : wchar(wchar_), start(start_), end(end_) {
}


/////////////
// methods //
/////////////
std::vector<std::shared_ptr<Char>> Char::characterize(const char* text) {
  HANAL_ASSERT(text != nullptr, "Null text to characteraize");
  auto en_US_utf8 = std::locale("en_US.UTF-8");
  auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(en_US_utf8);
  auto mbst = std::mbstate_t();
  const char* from_next = nullptr;
  std::vector<std::shared_ptr<Char>> chars;
  for (const char* from_curr = text; *from_curr != '\0'; from_curr = from_next) {
    wchar_t wchar[2] = L"";
    wchar_t* to_next = nullptr;
    auto result = facet.in(mbst, from_curr, from_curr + 6, from_next, wchar, wchar + 1, to_next);
    HANAL_ASSERT(result == std::codecvt_base::partial || result == std::codecvt_base::ok,
                 "Fail to convert character at position: " + boost::lexical_cast<std::string>(from_next - from_curr));
    chars.emplace_back(std::make_shared<Char>(wchar[0], from_curr, from_next));
  }
  return chars;
}


}    // namespace hanal
