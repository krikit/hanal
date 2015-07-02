/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/hanal_api.h"


//////////////
// includes //
//////////////
#include <memory>
#include <vector>

#include "boost/assert.hpp"
#include "hanal/Except.hpp"
#include "hanal/HanalApi.hpp"


///////////////
// variables //
///////////////
std::vector<std::shared_ptr<hanal::HanalApi>> _handles{ hanal::HanalApi::create() };


/////////////
// methods //
/////////////
const char* hanal_version() {
  return HANAL_VERSION;
}


int hanal_open(const char* rsc_dir, const char* opt_str) {
  // TODO(krikit): use mutex and lock for thread safety
  auto hanal_api = hanal::HanalApi::create();
  try {
    hanal_api->open(rsc_dir, opt_str);
    _handles.push_back(hanal_api);
  } catch (hanal::Except& exc) {
    return -1;
    // TODO(krikit): there should be method to notice error message
  }
  return _handles.size();
}


void hanal_close(int handle) {
  _handles[handle].reset();
}


const char* hanal_pos_tag(int handle, const char* sent, const char* opt_str) {
  BOOST_ASSERT(handle > 0 && handle < _handles.size());
  auto hanal_api = _handles[handle];
  return hanal_api->pos_tag(sent, opt_str).c_str();
}
