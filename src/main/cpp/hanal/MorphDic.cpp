/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/MorphDic.hpp"


//////////////
// includes //
//////////////
#include <string>

#include "boost/log/trivial.hpp"


namespace hanal {


MorphDic::~MorphDic() {
  close();
}


void MorphDic::open(std::string rsc_dir) {
  close();
  _trie.open(rsc_dir + "/morph.trie");
  _value.open(rsc_dir + "/morph.val");
  auto val_data = _value.data();

  MappedDic<int16_t> len;    // this contains length of each value text
  len.open(rsc_dir + "/morph.val.len");
  auto len_data = len.data();

  int size = len.size();
  _val_idx.reserve(size);
  _val_idx.push_back(val_data);
  int len_sum = len_data[0];
  for (int i = 1; i < size; ++i) {
    _val_idx.push_back(val_data + len_sum);
    len_sum += len_data[i];    // add each length (length already includes zero termination)
  }

  HANAL_ASSERT(_value.size() == len_sum, "Invalid morpheme dic at resource dir: " + rsc_dir);
  BOOST_LOG_TRIVIAL(info) << "Morpheme dictionary loaded";
}


void MorphDic::close() {
  _trie.close();
  _value.close();
  _val_idx.clear();
}


}    // namespace hanal
