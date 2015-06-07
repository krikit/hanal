/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_TRIE_HPP
#define HANAL_TRIE_HPP


//////////////
// includes //
//////////////
#include <string>

#include "boost/iostreams/device/mapped_file.hpp"
#include "boost/optional.hpp"


namespace hanal {


/**
 * trie for wide character string
 */
class Trie {
 public:
  virtual ~Trie();

  /**
   * @brief        open resource file
   * @param  path  file path
   */
  void open(std::string path);

  void close();    ///< close resource file

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


 private:
  typedef struct {
    wchar_t ch = 0;    ///< (wide) character
    int32_t val_idx = -1;    ///< index of value
    int32_t child_start = -1;    ///< child node start from this node
    int32_t child_num = -1;    ///< number of children
  } node_t;

  boost::iostreams::mapped_file_source _mapped_file;    ///< mmap file
  const node_t* _root_node = nullptr;    ///< root node of trie

  /*
   * @brief         find value with given key start with node
   * @param   key   key string
   * @param   node  start node
   * @return        value index. boost::none for non-existing key
   */
  boost::optional<int> _find(const wchar_t* key, const node_t* node);
};


}    // namespace hanal


#endif  // HANAL_TRIE_HPP
