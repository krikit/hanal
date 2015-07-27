/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Word.hpp"


//////////////
// includes //
//////////////
#include <algorithm>
#include <list>
#include <vector>
#include <set>

#include "hanal/Char.hpp"
#include "hanal/MorphDic.hpp"
#include "hanal/ViterbiTrellis.hpp"


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
Word::Word(SHDPTR(Char) char_, int char_idx_)
    : chars(1, char_), char_idx(char_idx_) {
}


/////////////
// methods //
/////////////
SHDPTRVEC(Word) Word::tokenize(const char* text) {
  auto chars_ = Char::characterize(text);
  SHDPTRVEC(Word) words;
  int char_idx = 0;
  bool is_in_space = true;
  for (auto& char_ : chars_) {
    if (char_->is_space()) {
      is_in_space = true;
    } else {
      if (is_in_space) {
        // first character => start of word
        words.emplace_back(std::make_shared<Word>(char_, char_idx));
      } else {
        words.back()->chars.emplace_back(char_);
      }
      is_in_space = false;
      char_idx += 1;
    }
  }
  return words;
}


/////////////
// methods //
/////////////
int Word::char_len(const SHDPTRVEC(Word)& words) {
  int length_sum = 0;
  for (auto& word : words) {
    length_sum += word->chars.size();
  }
  return length_sum;
}


std::wstring Word::to_wstr() {
  std::wstring text;
  std::transform(chars.begin(), chars.end(), text.begin(),
                 [](const SHDPTR(Char)& char_) { return char_->wchar; });
  return text;
}


std::wstring Word::to_wstr_reversed() {
  std::wostringstream wss;
  for (auto iter = chars.rbegin(); iter != chars.rend(); ++iter) {
    wss << (*iter)->wchar;
  }
  return wss.str();
}


void Word::analyze_forward(MorphDic* morph_dic, ViterbiTrellis* trellis, int trellis_idx) {
  std::wstring text = to_wstr();
  std::set<int> lookup_starts = {0};
  while (lookup_starts.size() > 0) {
    int lookup_start = *lookup_starts.begin();
    auto matches = morph_dic->lookup(&text[lookup_start]);
    if (matches.size() == 0) {
      // TODO(krikit): we should handle unknown words
      HANAL_THROW("Not implemented yet");
    } else {
      for (auto& match : matches) {
        for (auto& anal_result : morph_dic->value(match.val_idx)) {
          trellis->add_node(anal_result, trellis_idx + lookup_start, match.len);
        }
        if ((lookup_start + match.len) < text.length()) lookup_starts.insert(lookup_start + match.len);
      }
    }
    lookup_starts.erase(lookup_starts.begin());
  }
}


void Word::analyze_backward(MorphDic* morph_dic, ViterbiTrellis* trellis, int trellis_idx) {
  std::wstring text_reversed = to_wstr_reversed();
  // TODO(krikit): analyze reversed direction (at the end of word)
  HANAL_THROW("Not implemented yet");
}


Word Word::operator+(const Word& that) {
  HANAL_ASSERT(this->char_idx + this->chars.size() == that.char_idx, "Can merge only adjacent words");
  Word merged(*this);
  merged.chars.insert(merged.chars.end(), that.chars.begin(), that.chars.end());
  return merged;
}


}    // namespace hanal
