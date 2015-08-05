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
  std::wostringstream wss;
  for (auto& char_ : chars) {
    wss << char_->wchar;
  }
  return wss.str();
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
      for (auto& match_len : _estimate_unk_word_forward(trellis, trellis_idx, lookup_start)) {
        if ((lookup_start + match_len) < text.length()) lookup_starts.insert(lookup_start + match_len);
      }
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


std::set<int> Word::_estimate_unk_word_forward(ViterbiTrellis* trellis, int trellis_idx, int lookup_start) {
  std::set<int> match_lengths;

  auto first_char = chars[lookup_start];
  auto begin = chars.begin() + lookup_start;
  auto end = chars.end();
  if (Char::merge_strategy(first_char->type()) == Char::MergeStrategy::SEPARATELY) {
    // only single character
    end = begin + 1;
  } else if (Char::merge_strategy(first_char->type()) == Char::MergeStrategy::BY_CHAR) {
    // find end of lexically same characters
    auto char_merge_pred = [&first_char] (const SHDPTR(Char)& merge_char) {
        return first_char->wchar == merge_char->wchar;
    };
    end = std::find_if_not(begin + 1, chars.end(), char_merge_pred);
  } else if (Char::merge_strategy(first_char->type()) == Char::MergeStrategy::BY_TYPE) {
    // find end of same type characters
    auto type_merge_pred = [&first_char] (const SHDPTR(Char)& merge_char) {
        return first_char->type() == merge_char->type();
    };
    end = std::find_if_not(begin + 1, chars.end(), type_merge_pred);
  }

  int match_length = end - begin;
  _add_unk_word(trellis, trellis_idx, lookup_start, match_length);
  match_lengths.insert(match_length);

  if (first_char->type() == Char::Type::HANGUL) {
    // add more estimated words for Hangul. 1, 2 and (match_length - 1)
    if (match_length > 1) _add_unk_word(trellis, trellis_idx, lookup_start, 1);
    if (match_length > 2) _add_unk_word(trellis, trellis_idx, lookup_start, 2);
    if (match_length > 3) _add_unk_word(trellis, trellis_idx, lookup_start, match_length - 1);
  }

  return match_lengths;
}


void Word::_add_unk_word(ViterbiTrellis* trellis, int trellis_idx, int lookup_start, int length) {
  std::unique_ptr<wchar_t[]> lex(new wchar_t[length + 1]);
  for (int idx = 0; idx < length; ++idx) {
    lex[idx] = chars[lookup_start + idx]->wchar;
  }
  SHDPTR(Morph) morph = std::make_shared<Morph>(std::move(lex), chars[lookup_start]->estimate_pos_tag());
  SHDPTRVEC(Morph) estimated_result;
  estimated_result.emplace_back(morph);
  trellis->add_node(estimated_result, trellis_idx + lookup_start, length);
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
