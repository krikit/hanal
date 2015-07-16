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
  const char* text1 = u8" a \t\u00E0\u3000\uD558\uD558 \n";
  auto words1 = hanal::Word::tokenize(text1);
  EXPECT_EQ(3, words1.size());
  EXPECT_EQ(1, words1[0]->chars.size());    // "a"
  EXPECT_EQ(text1 + 1, words1[0]->chars.front()->start);
  EXPECT_EQ(text1 + 2, words1[0]->chars.back()->end);
  EXPECT_EQ(1, words1[1]->chars.size());    // "à"
  EXPECT_EQ(text1 + 4, words1[1]->chars.front()->start);
  EXPECT_EQ(text1 + 6, words1[1]->chars.back()->end);
  EXPECT_EQ(2, words1[2]->chars.size());    // "하하"
  EXPECT_EQ(text1 + 9, words1[2]->chars.front()->start);
  EXPECT_EQ(text1 + 15, words1[2]->chars.back()->end);

  const char* text2 = "";
  auto words2 = hanal::Word::tokenize(text2);
  EXPECT_EQ(0, words2.size());

  const char* text3 = " \v\r";
  auto words3 = hanal::Word::tokenize(text3);
  EXPECT_EQ(0, words3.size());

  EXPECT_THROW(hanal::Word::tokenize(nullptr), hanal::Except);
}
