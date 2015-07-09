/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/TransMat.hpp"


//////////////
// includes //
//////////////
#include <string>


namespace hanal {


/////////////
// methods //
/////////////
float TransMat::get(SejongTag from, SejongTag to) {
  int size = static_cast<int>(SejongTag::_SIZE);
  int from_ = static_cast<int>(from);
  HANAL_ASSERT(from_ >= 0 && from_ < size, "Invalid from-tag ID: " + boost::lexical_cast<std::string>(from_));
  int to_ = static_cast<int>(to);
  HANAL_ASSERT(to_ >= 0 && to_ < size, "Invalid to-tag ID: " + boost::lexical_cast<std::string>(to_));
  return data()[to_ * size + from_];
}


}    // namespace hanal
