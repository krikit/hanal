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
#include "hanal/macro.hpp"
#include "hanal/MorphDic.hpp"
#include "hanal/Option.hpp"
#include "hanal/StateFeatDic.hpp"
#include "hanal/TransMat.hpp"


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
HanalImpl::HanalImpl() : _morph_dic(std::make_shared<MorphDic>()), _state_feat_dic(std::make_shared<StateFeatDic>()),
                         _trans_mat(std::make_shared<TransMat>()) {
}


HanalImpl::~HanalImpl() {
  close();
}


/////////////
// methods //
/////////////
SHDPTR(HanalApi) HanalApi::create() {
  return std::make_shared<HanalImpl>();
}


void HanalImpl::open(std::string rsc_dir, std::string opt_str) {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  _option = std::make_shared<Option>(opt_str);
  _morph_dic->open(rsc_dir);
  _state_feat_dic->open(rsc_dir);
  _trans_mat->open(rsc_dir + "/trans_mat.bin");
}


void HanalImpl::close() {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  _morph_dic->close();
  _state_feat_dic->close();
  _trans_mat->close();
}


const std::string& HanalImpl::pos_tag(const char* sent, const char* opt_str) {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  HANAL_THROW("Not implemented yet!");
}


}    // namespace hanal
