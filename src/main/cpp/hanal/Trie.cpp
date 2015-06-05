/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Trie.hpp"


//////////////
// includes //
//////////////
#include <string>

#include "boost/lexical_cast.hpp"
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
  _mapped_file.open(path);
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


}    // namespace hanal
