/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014 - 2015,
  krikit. All rights reserved. BSD 2 - Clause License
 */


#include "hanal/Option.hpp"


//////////////
// includes //
//////////////
#include <string>


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
Option::Option(std::string opt_str) {
  // TODO(krikit): parse option string
}


/////////////
// methods //
/////////////
Option Option::override(std::string opt_str) {
  // TODO(krikit): parse option string and return overrided options
  return *this;
}


}    // namespace hanal
