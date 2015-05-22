/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Trie.hpp"


//////////////
// includes //
//////////////
#include <string>
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
  if (!_mapped_file.is_open()) throw EXCEPT("Fail to open file: " + path);
}


void Trie::close() {
  _mapped_file.close();
}


}    // namespace hanal
