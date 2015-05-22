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

 private:
  boost::iostreams::mapped_file_source _mapped_file;    ///< mmap file
};


}    // namespace hanal


#endif  // HANAL_TRIE_HPP
