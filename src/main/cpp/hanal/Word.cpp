/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Word.hpp"


//////////////
// includes //
//////////////
#include <algorithm>
#include <vector>

#include "hanal/Char.hpp"


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
Word::Word(std::shared_ptr<Char> char_)
    : chars(1, char_) {
}


/////////////
// methods //
/////////////
std::vector<std::shared_ptr<Word>> Word::tokenize(const char* text) {
  auto chars_ = Char::characterize(text);
  std::vector<std::shared_ptr<Word>> words;
  bool is_in_space = true;
  for (auto& char_ : chars_) {
    if (char_->is_space()) {
      is_in_space = true;
    } else {
      if (is_in_space) {
        // first character => start of word
        words.push_back(std::make_shared<Word>(char_));
      } else {
        words.back()->chars.push_back(char_);
      }
      is_in_space = false;
    }
  }
  return words;
}


}    // namespace hanal
