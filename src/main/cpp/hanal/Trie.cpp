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


/////////////
// methods //
/////////////
std::string _trie_node_t::str(const _trie_node_t* root_node) const {
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
  MappedDic<_trie_node_t>::open(path, false);

#if defined(TRACE) && defined(DEBUG)
  const _trie_node_t* root_node = data();
  for (int i = 0; i < sizeof(root_node) / sizeof(_trie_node_t); ++i) {
    BOOST_LOG_TRIVIAL(trace) << root_node[i].str(root_node);
  }
#endif
}


boost::optional<int> Trie::find(const std::wstring& key) const {
  return find(key.c_str());
}


boost::optional<int> Trie::find(const wchar_t* key) const {
  HANAL_ASSERT(key != nullptr, "Null key");
  if (*key == L'\0') return boost::none;
  return _find(key, const_data());
}


std::list<Trie::match_t> Trie::search_common_prefix_matches(const std::wstring& text) const {
  return search_common_prefix_matches(text.c_str());
}


std::list<Trie::match_t> Trie::search_common_prefix_matches(const wchar_t* text) const {
  HANAL_ASSERT(text != nullptr, "Null text");
  std::list<match_t> found;
  _search(text, const_data(), &found, 0);
  return found;
}


boost::optional<int> Trie::_find(const wchar_t* key, const _trie_node_t* node) const {
  BOOST_LOG_TRIVIAL(trace) << "key: [" << key << "], " << node->str(const_data());
  if (node->child_start <= 0 || node->child_num <= 0) return boost::none;
  auto begin = node + node->child_start;
  auto end = begin + node->child_num;
  auto pred = [&key] (const _trie_node_t& _node) { return _node.ch == *key; };
  auto found_node = std::find_if(begin, end, pred);
  if (found_node == end) {
    BOOST_LOG_TRIVIAL(trace) << "  not found";
    return boost::none;
  } else {
    BOOST_LOG_TRIVIAL(trace) << "  found: " << found_node->str(const_data());
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


void Trie::_search(const wchar_t* text, const _trie_node_t* node, std::list<Trie::match_t>* matches, int len) const {
  BOOST_LOG_TRIVIAL(trace) << "text(" << len << "): [" << text << "], " << node->str(const_data());
  if (*text == '\0' || node->child_start <= 0 || node->child_num <= 0) return;
  auto begin = node + node->child_start;
  auto end = begin + node->child_num;
  auto pred = [&text] (const _trie_node_t& _node) { return _node.ch == *text; };
  auto match_node = std::find_if(begin, end, pred);
  if (match_node == end) {
    BOOST_LOG_TRIVIAL(trace) << "  not matched";
    return;
  } else {
    BOOST_LOG_TRIVIAL(trace) << "  matched: " << match_node->str(const_data());
    if (match_node->val_idx >= 0) matches->emplace_back(len + 1, match_node->val_idx);
    _search(text + 1, match_node, matches, len + 1);
  }
}


}    // namespace hanal
