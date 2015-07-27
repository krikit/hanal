/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/MorphDic.hpp"


//////////////
// includes //
//////////////
#include <list>
#include <string>
#include <vector>

#include "boost/log/trivial.hpp"


namespace hanal {


MorphDic::~MorphDic() {
  close();
}


void MorphDic::open(std::string rsc_dir) {
  close();
  _trie.open(rsc_dir + "/morph.trie");
  _value.open(rsc_dir + "/morph.val", true);    // private mode open
  auto val_data = _value.data();

  MappedDic<int16_t> len;    // this contains length of each value text
  len.open(rsc_dir + "/morph.val.len");
  auto len_data = len.const_data();

  int size = len.size();
  _val_idx.reserve(size);
  _val_idx.emplace_back(val_data);
  int len_sum = len_data[0];
  for (int i = 1; i < size; ++i) {
    _val_idx.emplace_back(val_data + len_sum);
    len_sum += len_data[i];    // add each length (length already includes zero termination)
  }

  HANAL_ASSERT(_value.size() == len_sum, "Invalid morpheme dic at resource dir: " + rsc_dir);
  BOOST_LOG_TRIVIAL(info) << "Morpheme dictionary loaded";
}


void MorphDic::close() {
  _trie.close();
  _value.close();
  _val_idx.clear();
  _val_cache.clear();
}


std::list<Trie::match_t> MorphDic::lookup(const wchar_t* text) const {
  return _trie.search_common_prefix_matches(text);
}


const std::vector<SHDPTRVEC(Morph)>& MorphDic::value(int idx) {
  HANAL_ASSERT(0 <= idx && idx < _val_idx.size(), "Invalid value index: " + boost::lexical_cast<std::string>(idx));
  if (_val_cache.empty()) _val_cache.resize(_val_idx.size());
  if (_val_cache[idx].empty()) _val_cache[idx] = Morph::parse_anal_result_vec(_val_idx[idx]);
  return _val_cache[idx];
}


}    // namespace hanal
