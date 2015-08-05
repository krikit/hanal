/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Char.hpp"


//////////////
// includes //
//////////////
#include <locale>
#include <string>
#include <vector>

#include "boost/lexical_cast.hpp"
#include "hanal/Except.hpp"


namespace hanal {


////////////////////
// static members //
////////////////////
const std::wstring Char::SPACE = L" \t\v\r\n\u3000";


////////////////////
// ctors and dtor //
////////////////////
Char::Char(wchar_t wchar_, const char* start_, const char* end_)
    : wchar(wchar_), start(start_), end(end_) {
}


/////////////
// methods //
/////////////
SHDPTRVEC(Char) Char::characterize(const char* text) {
  HANAL_ASSERT(text != nullptr, "Null text to characteraize");
  auto en_US_utf8 = std::locale("en_US.UTF-8");
  auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(en_US_utf8);
  auto mbst = std::mbstate_t();
  const char* from_next = nullptr;
  SHDPTRVEC(Char) chars;
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


bool Char::is_space() {
  if (is_space(wchar)) _type = Type::SPACE;
  return _type == Type::SPACE;
}


Char::Type Char::type() {
  if (_type == Type::UNK) {
    if (is_space(wchar)) {
      _type = Type::SPACE;
    } else if (is_hangul(wchar)) {
      _type = Type::HANGUL;
    } else if (is_latin(wchar)) {
      _type = Type::LATIN;
    } else if (is_number(wchar)) {
      _type = Type::NUMBER;
    } else if (is_cjk(wchar)) {
      _type = Type::CJK;
    } else if (is_ellipsis(wchar)) {
      _type = Type::ELLIPSIS;
    } else if (is_period(wchar)) {
      _type = Type::PERIOD;
    } else if (is_o_mark(wchar)) {
      _type = Type::O_MARK;
    } else if (is_comma(wchar)) {
      _type = Type::COMMA;
    } else if (is_quote(wchar)) {
      _type = Type::QUOTE;
    } else if (is_symbol(wchar)) {
      _type = Type::SYMBOL;
    } else {
      _type = Type::FOREIGN;
    }
  }
  return _type;
}


Char::MergeStrategy Char::merge_strategy(Type type) {
  if (type == Type::COMMA || type == Type::QUOTE || type == Type::SYMBOL) {
    return MergeStrategy::SEPARATELY;
  } else if (type == Type::PERIOD || type == Type::ELLIPSIS || type == Type::O_MARK) {
    return MergeStrategy::BY_CHAR;
  } else {
    // SPACE, HANGUL, LATIN, NUMBER, CJK, FOREIGN
    return MergeStrategy::BY_TYPE;
  }
}


SejongTag Char::estimate_pos_tag() {
  return estimate_pos_tag(type());
}


SejongTag Char::estimate_pos_tag(Type type) {
  if (type == Type::HANGUL) {
    return SejongTag::NNG;
  } else if (type == Type::LATIN || type == Type::FOREIGN) {
    return SejongTag::SL;
  } else if (type == Type::NUMBER) {
    return SejongTag::SN;
  } else if (type == Type::CJK) {
    return SejongTag::SH;
  } else if (type == Type::ELLIPSIS) {
    return SejongTag::SE;
  } else if (type == Type::PERIOD) {
    return SejongTag::SF;
  } else if (type == Type::O_MARK) {
    return SejongTag::SO;
  } else if (type == Type::COMMA) {
    return SejongTag::SP;
  } else if (type == Type::QUOTE) {
    return SejongTag::SS;
  } else if (type == Type::SYMBOL) {
    return SejongTag::SW;
  } else {
    HANAL_THROW("Character type is unknown or space!");
  }
}


bool Char::is_space(wchar_t wchar) {
  return SPACE.find(wchar) != std::wstring::npos;
}


bool Char::is_hangul(wchar_t wchar) {
  if (L'\uAC00' <= wchar && wchar <= L'\uD7AF') return true;    // Hangul syllables
  if (L'\u1100' <= wchar && wchar <= L'\u11FF') return true;    // Hangul jamo (consonants and vowels)
  return false;
}


bool Char::is_latin(wchar_t wchar) {
  if (L'A' <= wchar && wchar <= L'Z') return true;
  if (L'\uFF21' <= wchar && wchar <= L'\uFF3A') return true;    // full-width upper case alphabets
  if (L'a' <= wchar && wchar <= L'z') return true;
  if (L'\uFF41' <= wchar && wchar <= L'\uFF5A') return true;    // full-width lower case alphabets
  // Latin-1 supplement
  if (L'\u00C0' <= wchar && wchar <= L'\u00FF' && wchar != L'\u00D7' && wchar != L'\u00F7') return true;
  return false;
}


bool Char::is_number(wchar_t wchar) {
  if (L'0' <= wchar && wchar <= L'9') return true;
  if (L'\uFF10' <= wchar && wchar <= L'\uFF19') return true;    // full-width numbers
  return false;
}


bool Char::is_cjk(wchar_t wchar) {
  if (L'\u4E00' <= wchar && wchar <= L'\u9FD5') return true;    // CJK unified ideographs
  if (L'\uF900' <= wchar && wchar <= L'\uFAD9') return true;    // CJK compatibility ideographs
  if (L'\u2F00' <= wchar && wchar <= L'\u2FDF') return true;    // CJK radicals
  return false;
}


bool Char::is_foreign(wchar_t wchar) {
  return !is_space(wchar) && !is_hangul(wchar) && !is_latin(wchar) && !is_number(wchar) && !is_cjk(wchar)
      && !is_ellipsis(wchar) && !is_period(wchar) && !is_o_mark(wchar) && !is_comma(wchar) && !is_quote(wchar)
      && !is_symbol(wchar);
}


bool Char::is_ellipsis(wchar_t wchar) {
  if (wchar == L'\u2026') return true;    // …
  return false;
}


bool Char::is_period(wchar_t wchar) {
  if (wchar == L'.' || wchar == L'\uFF0E' || wchar == L'\uFF61') return true;    // .．｡
  if (wchar == L'?' || wchar == L'\uFF1F') return true;    // ?？
  if (wchar == L'!' || wchar == L'\uFF01') return true;    // !！
  return false;
}


bool Char::is_o_mark(wchar_t wchar) {
  if (wchar == L'~' || wchar == L'\uFF5E') return true;    // ~
  if (wchar == L'\u25CB' || wchar == L'\u25EF') return true;    // ○◯
  if (wchar == L'\u25A1') return true;    // □
  return false;
}


bool Char::is_comma(wchar_t wchar) {
  if (wchar == L',' || wchar == L'\uFF0C' || wchar == L'\uFF64') return true;    // ,，､
  if (wchar == L'\uFF65') return true;    // ･ (middle dot)
  if (wchar == L':' || wchar == L'\uFF1A') return true;    // :：
  if (wchar == L';' || wchar == L'\uFF1B') return true;    // ;；
  if (wchar == L'/' || wchar == L'\uFF0F') return true;    // /／
  return false;
}


bool Char::is_quote(wchar_t wchar) {
  if (wchar == L'\'' || wchar == L'\uFF07' || wchar == L'\u2018' || wchar == L'\u2019') return true;    // '＇‘’
  if (wchar == L'"' || wchar == L'\uFF02' || wchar == L'\u201C' || wchar == L'\u201D') return true;    // "＂“”
  if (wchar == L'-' || wchar == L'\uFF0D') return true;    // -－
  if (wchar == L'(' || wchar == L')' || wchar == L'\uFF08' || wchar == L'\uFF09') return true;    // ()（）
  if (wchar == L'[' || wchar == L']' || wchar == L'\uFF3B' || wchar == L'\uFF3D') return true;    // []［］
  if (wchar == L'{' || wchar == L'}' || wchar == L'\uFF5B' || wchar == L'\uFF5D') return true;    // {}｛｝
  if (wchar == L'<' || wchar == L'>' || wchar == L'\uFF1C' || wchar == L'\uFF1E') return true;    // <>＜＞
  return false;
}


bool Char::is_symbol(wchar_t wchar) {
  if (wcschr(L"`@#$%^&*_=+|\\", wchar) != nullptr) return true;
  if (wcschr(L"\uFF03\uFF04\uFF05\uFF06\uFF0A\uFF0B\uFF1D\uFF20\uFF3C\uFF3E\uFF3F\uFF40\uFF5C", wchar) != nullptr) {
    //           ＃    ＄     ％    ＆    ＊     ＋    ＝     ＠    ＼    ＾     ＿    ｀    ｜
    return true;
  }
  return false;
}


}    // namespace hanal
