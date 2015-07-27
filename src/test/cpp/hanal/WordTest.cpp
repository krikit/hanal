/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


//////////////
// includes //
//////////////
#include "gtest/gtest.h"
#include "hanal/Char.hpp"
#include "hanal/Except.hpp"
#include "hanal/Word.hpp"


/**
 * test fixture for Word
 */
class WordTest: public testing::Test {
};


TEST_F(WordTest, tokenize) {
  const char* text1 = u8" a \t\u00E0\u00FF\u3000\uD558\uD558\uD558 \n";    // "a", "àÿ", "하하하"
  auto words1 = hanal::Word::tokenize(text1);
  EXPECT_EQ(3, words1.size());
  EXPECT_EQ(1, words1[0]->chars.size());    // "a"
  EXPECT_EQ(0, words1[0]->char_idx);
  EXPECT_EQ(text1 + 1, words1[0]->chars.front()->start);
  EXPECT_EQ(text1 + 2, words1[0]->chars.back()->end);
  EXPECT_EQ(2, words1[1]->chars.size());    // "àÿ"
  EXPECT_EQ(1, words1[1]->char_idx);
  EXPECT_EQ(text1 + 4, words1[1]->chars.front()->start);
  EXPECT_EQ(text1 + 8, words1[1]->chars.back()->end);
  EXPECT_EQ(3, words1[2]->chars.size());    // "하하하"
  EXPECT_EQ(3, words1[2]->char_idx);
  EXPECT_EQ(text1 + 11, words1[2]->chars.front()->start);
  EXPECT_EQ(text1 + 20, words1[2]->chars.back()->end);

  const char* text2 = "";
  auto words2 = hanal::Word::tokenize(text2);
  EXPECT_EQ(0, words2.size());

  const char* text3 = " \v\r";
  auto words3 = hanal::Word::tokenize(text3);
  EXPECT_EQ(0, words3.size());

  EXPECT_THROW(hanal::Word::tokenize(nullptr), hanal::Except);
}


TEST_F(WordTest, length) {
  const char* text1 = u8" a \t\u00E0\u00FF\u3000\uD558\uD558\uD558 \n";    // "a", "àÿ", "하하하"
  auto words1 = hanal::Word::tokenize(text1);
  EXPECT_EQ(6, hanal::Word::char_len(words1));

  const char* text2 = "";
  auto words2 = hanal::Word::tokenize(text2);
  EXPECT_EQ(0, hanal::Word::char_len(words2));

  const char* text3 = " \v\r";
  auto words3 = hanal::Word::tokenize(text3);
  EXPECT_EQ(0, hanal::Word::char_len(words3));
}


TEST_F(WordTest, to_wstr) {
  const char* text1 = u8" a \t\u00E0\u00FF\u3000\uD558\uD558\uD558 \n";    // "a", "àÿ", "하하하"
  auto words1 = hanal::Word::tokenize(text1);
  EXPECT_STREQ(L"a", words1[0]->to_wstr().c_str());
  EXPECT_STREQ(L"\u00E0\u00FF", words1[1]->to_wstr().c_str());    // "àÿ"
  EXPECT_STREQ(L"\uD558\uD558\uD558", words1[2]->to_wstr().c_str());    // "하하하"
}


TEST_F(WordTest, to_wstr_reversed) {
  const char* text1 = u8" a \t\u00E0\u00FF\u3000\uAC00\uB098\uB2EF \n";    // "a", "àÿ", "가나다"
  auto words1 = hanal::Word::tokenize(text1);
  EXPECT_STREQ(L"a", words1[0]->to_wstr_reversed().c_str());
  EXPECT_STREQ(L"\u00FF\u00E0", words1[1]->to_wstr_reversed().c_str());    // "ÿà"
  EXPECT_STREQ(L"\uB2EF\uB098\uAC00", words1[2]->to_wstr_reversed().c_str());    // "다나가"
}
