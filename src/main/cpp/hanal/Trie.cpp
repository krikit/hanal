/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Trie.hpp"


//////////////
// includes //
//////////////
#include <algorithm>
#include <list>
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
std::string Trie::_node_t::str(const Trie::_node_t* root_node) const {
  std::ostringstream oss;
  oss << "node[" << (this - root_node) << "]{'";
  if (ch == 0) {
    oss << "ROOT";
  } else {
    oss << static_cast<char>(ch);
  }
  oss << "', " << val_idx << ", (" << child_start << ", " << child_num << ")}";
  return oss.str();
}


void Trie::open(std::string path) {
  close();
  try {
    _mapped_file.open(path);
  } catch (std::exception exc) {
    HANAL_THROW(exc.what());
  }
  if (!_mapped_file.is_open()) HANAL_THROW("Fail to open file: " + path);
  if ((_mapped_file.size() > 0) && ((_mapped_file.size() % sizeof(_node_t) != 0))) {
    close();
    HANAL_THROW("Invalid size of file: " + boost::lexical_cast<std::string>(_mapped_file.size()));
  }
  _root_node = reinterpret_cast<const _node_t*>(_mapped_file.data());

#if defined(TRACE) && defined(DEBUG)
  for (int i = 0; i < _mapped_file.size() / sizeof(_node_t); ++i) {
    BOOST_LOG_TRIVIAL(trace) << _root_node[i].str(_root_node);
  }
#endif
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
  if (*key == L'\0') return boost::none;
  return _find(key, _root_node);
}


std::list<Trie::match_t> Trie::search_common_prefix_matches(const std::wstring &text) {
  return search_common_prefix_matches(text.c_str());
}


std::list<Trie::match_t> Trie::search_common_prefix_matches(const wchar_t *text) {
  if (text == nullptr) HANAL_THROW("Null text");
  std::list<match_t> found;
  _search(text, _root_node, &found, 0);
  return found;
}


boost::optional<int> Trie::_find(const wchar_t* key, const _node_t* node) {
  BOOST_LOG_TRIVIAL(trace) << "key: [" << key << "], " << node->str(_root_node);
  if (node->child_start <= 0 || node->child_num <= 0) return boost::none;
  auto begin = node + node->child_start;
  auto end = begin + node->child_num;
  auto pred = [&key] (const _node_t& _node) { return _node.ch == *key; };
  auto found_node = std::find_if(begin, end, pred);
  if (found_node == end) {
    BOOST_LOG_TRIVIAL(trace) << "  not found";
    return boost::none;
  } else {
    BOOST_LOG_TRIVIAL(trace) << "  found: " << found_node->str(_root_node);
    key += 1;
    if (*key == L'\0') {
      if (found_node->val_idx >= 0) {
        return boost::optional<int>(found_node->val_idx);
      } else {
        return boost::none;
      }
    } else {
      return _find(key, found_node);
    }
  }
}


void Trie::_search(const wchar_t* text, const _node_t* node, std::list<Trie::match_t>* matches, int len) {
  BOOST_LOG_TRIVIAL(trace) << "text(" << len << "): [" << text << "], " << node->str(_root_node);
  if (*text == '\0' || node->child_start <= 0 || node->child_num <= 0) return;
  auto begin = node + node->child_start;
  auto end = begin + node->child_num;
  auto pred = [&text] (const _node_t& _node) { return _node.ch == *text; };
  auto match_node = std::find_if(begin, end, pred);
  if (match_node == end) {
    BOOST_LOG_TRIVIAL(trace) << "  not matched";
    return;
  } else {
    BOOST_LOG_TRIVIAL(trace) << "  matched: " << match_node->str(_root_node);
    if (match_node->val_idx >= 0) matches->emplace_back(len + 1, match_node->val_idx);
    _search(text + 1, match_node, matches, len + 1);
  }
}


}    // namespace hanal
