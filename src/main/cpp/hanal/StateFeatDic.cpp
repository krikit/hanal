/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/StateFeatDic.hpp"


//////////////
// includes //
//////////////
#include <string>

#include "boost/log/trivial.hpp"


namespace hanal {


StateFeatDic::~StateFeatDic() {
  close();
}


void StateFeatDic::open(std::string rsc_dir) {
  close();
  _trie.open(rsc_dir + "/state_feat.trie");
  _value.open(rsc_dir + "/state_feat.val");
  BOOST_LOG_TRIVIAL(info) << "State-features dictionary loaded";
}


void StateFeatDic::close() {
  _trie.close();
  _value.close();
}


float StateFeatDic::get(SejongTag state, const wchar_t* feat) {
  std::wostringstream oss;
  wchar_t state_char = L'A' + static_cast<int>(state);
  oss << state_char << feat;
  auto idx = _trie.find(oss.str());
  if (idx) return _value.data()[*idx];
  return 0.0;
}


}    // namespace hanal
