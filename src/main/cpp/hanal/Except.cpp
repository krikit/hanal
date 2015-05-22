/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Except.hpp"


//////////////
// includes //
//////////////
#include <sstream>
#include <string>


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
Except::Except(std::string msg, const char* file, const int line, const char* func)
    : _msg(msg), _file(file), _line(line), _func(func) {
}


/////////////
// methods //
/////////////
const char* Except::what() const noexcept {
  return _msg.c_str();
}


std::string Except::debug() {
  std::ostringstream oss;
  if (_func != nullptr) oss << _func;
  if (_file != nullptr) {
    oss <<  "(" << _file;
    if (_line > 0) oss << ":" << _line;
    oss << ")";
  }
  if (oss.str().length() > 0) oss << " ";
  oss << _msg;
  return oss.str();
}


}    // namespace hanal
