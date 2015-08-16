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
#include "hanal/ViterbiTrellis.hpp"
#include "hanal/Word.hpp"


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
  Option runtime_opt = _option->override(opt_str);
  auto words = Word::tokenize(sent);
  ViterbiTrellis trellis(words);
  for (int idx = 0; idx < words.size(); ++idx) {
    auto merged_word = *words[idx];
    for (int merge_num = 1; merge_num < runtime_opt.word_merge; ++merge_num) {
      merged_word += *words[idx + merge_num];
    }
    merged_word.analyze_forward(_morph_dic.get(), &trellis, merged_word.char_idx);
    // if (runtime_opt.anal_back) merged_word.analyze_backward(_morph_dic.get(), &trellis, merged_word.char_idx);
  }
  return _cache(trellis.str());    ///< TODO(krikit): return reference to the internal string buffer
}


const std::string& HanalImpl::_cache(std::string str) {
  _str_buf.emplace_back(std::move(str));
  if (_str_buf.size() > _CACHE_MAX) _str_buf.pop_front();
  return _str_buf.back();
}


}    // namespace hanal
