/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/hanal_api.h"


//////////////
// includes //
//////////////
#include <memory>
#include <mutex>    // NOLINT
#include <string>
#include <vector>

#include "boost/lexical_cast.hpp"
#include "hanal/Except.hpp"
#include "hanal/HanalApi.hpp"
#include "hanal/macro.hpp"


///////////////
// variables //
///////////////
SHDPTRVEC(hanal::HanalApi) _handles{ hanal::HanalApi::create() };    // handles
std::recursive_mutex _mutex;    // mutex to exclusively access handles


/////////////
// methods //
/////////////
const char* hanal_version() {
  return HANAL_VERSION;
}


int hanal_open(const char* rsc_dir, const char* opt_str) {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  auto hanal_api = hanal::HanalApi::create();
  try {
    hanal_api->open(rsc_dir, opt_str);
    _handles.emplace_back(hanal_api);
  } catch (hanal::Except& exc) {
    return -1;
    // TODO(krikit): there should be method to notice error message
  }
  return static_cast<int>(_handles.size());
}


void hanal_close(int handle) {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  _handles[handle].reset();
}


const char* hanal_pos_tag(int handle, const char* sent, const char* opt_str) {
  HANAL_ASSERT(handle > 0 && handle < _handles.size(), "Invalid handle" + boost::lexical_cast<std::string>(handle));
  auto hanal_api = _handles[handle];
  return hanal_api->pos_tag(sent, opt_str).c_str();
}
