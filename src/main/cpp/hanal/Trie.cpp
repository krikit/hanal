/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Trie.hpp"


//////////////
// includes //
//////////////
#include <algorithm>
#include <string>

#include "boost/lexical_cast.hpp"
#include "boost/log/trivial.hpp"
#include "hanal/Except.hpp"


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
Trie::~Trie() {
  close();
}


/////////////
// methods //
/////////////
void Trie::open(std::string path) {
  close();
  try {
    _mapped_file.open(path);
  } catch (std::exception exc) {
    HANAL_THROW(exc.what());
  }
  if (!_mapped_file.is_open()) HANAL_THROW("Fail to open file: " + path);
  if ((_mapped_file.size() > 0) && ((_mapped_file.size() % sizeof(node_t) != 0))) {
    close();
    HANAL_THROW("Invalid size of file: " + boost::lexical_cast<std::string>(_mapped_file.size()));
  }
  _root_node = reinterpret_cast<const node_t*>(_mapped_file.data());
}


void Trie::close() {
  _mapped_file.close();
  _root_node = nullptr;
}


boost::optional<int> Trie::find(const std::wstring& key) {
  return find(key.c_str());
}


boost::optional<int> Trie::find(const wchar_t* key) {
  if (key == nullptr) HANAL_THROW("Null key");
  if (*key == '\0') HANAL_THROW("Zero length key");
  return _find(key, _root_node);
}


boost::optional<int> Trie::_find(const wchar_t* key, const node_t* node) {
  BOOST_LOG_TRIVIAL(trace) << "key: [" << key << "], node: " << (node - _root_node);
  if (*key == '\0' && node->val_idx >= 0) return boost::optional<int>(node->val_idx);
  if (node->child_start <= 0 || node->child_num <= 0) return boost::none;
  auto begin = node + node->child_start;
  auto end = begin + node->child_num;
  auto pred = [&key] (const node_t& _node) { return _node.ch == *key; };
  auto match_node = std::find_if(begin, end, pred);
  if (match_node == end) {
    BOOST_LOG_TRIVIAL(trace) << "  not found";
    return boost::none;
  } else {
    BOOST_LOG_TRIVIAL(trace) << "  found: " << (match_node - _root_node);
  }
  return _find(key + 1, match_node);
}


}    // namespace hanal
