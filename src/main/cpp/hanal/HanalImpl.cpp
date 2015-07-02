/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/HanalImpl.hpp"


//////////////
// includes //
//////////////
#include <string>

#include "hanal/Except.hpp"


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
HanalImpl::~HanalImpl() {
  close();
}


/////////////
// methods //
/////////////
std::shared_ptr<HanalApi> HanalApi::create() {
  return std::make_shared<HanalImpl>();
}


void HanalImpl::open(std::string rsc_dir, std::string opt_str) {
  HANAL_THROW("Not implemented yet!");
}

void HanalImpl::close() {
  HANAL_THROW("Not implemented yet!");
}


const std::string& HanalImpl::pos_tag(const char* sent, const char* opt_str) {
  HANAL_THROW("Not implemented yet!");
}


}    // namespace hanal
