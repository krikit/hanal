/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_TRIE_HPP
#define HANAL_TRIE_HPP


//////////////
// includes //
//////////////
#include <list>
#include <string>

#include "boost/optional.hpp"
#include "hanal/MappedDic.hpp"


namespace hanal {


/**
 * node of morph_trie
 */
struct _node_t {
  wchar_t ch = 0;    ///< (wide) character
  int32_t val_idx = -1;    ///< index of value
  int32_t child_start = -1;    ///< child node start from this node
  int32_t child_num = -1;    ///< number of children
  std::string str(const _node_t* root_node) const;    ///< string representation for debugging
};


/**
 * morph_trie for wide character string
 */
class Trie: public MappedDic<_node_t> {
 public:
  struct match_t {
    int len;    ///< match length
    int val_idx;    ///< value index
    explicit match_t(int len = -1, int val_idx = -1): len(len), val_idx(val_idx) {}
  };

  virtual void open(std::string path);

  /*
   * @brief        find value index with given key
   * @param   key  key string
   * @return       value index. boost::none for non-existing key
   */
  boost::optional<int> find(const std::wstring& key);

  /*
   * @brief        find value index with given key
   * @param   key  key string
   * @return       value index. boost::none for non-existing key
   */
  boost::optional<int> find(const wchar_t* key);

  /*
   * @brief         search all entries until longest prefix
   * @param   text  text to search
   * @return        collection of match results
   */
  std::list<match_t> search_common_prefix_matches(const std::wstring &text);

  /*
   * @brief         search all entries until longest prefix
   * @param   text  text to search
   * @return        collection of match results
   */
  std::list<match_t> search_common_prefix_matches(const wchar_t *text);


 private:
  /*
   * @brief         find value with given key start with node
   * @param   key   key string
   * @param   node  start node
   * @return        value index. boost::none for non-existing key
   */
  boost::optional<int> _find(const wchar_t* key, const _node_t* node);

  /*
   * @brief           search text until not matching
   * @param  text     text to search
   * @param  node     start node
   * @param  matches  match list
   * @param  len      length proceeded
   */
  void _search(const wchar_t* text, const _node_t* node, std::list<match_t>* matches, int len);
};


}    // namespace hanal


#endif  // HANAL_TRIE_HPP
