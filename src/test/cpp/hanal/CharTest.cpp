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


/**
 * test fixture for Char
 */
class CharTest: public testing::Test {
};


TEST_F(CharTest, characterize) {
  const char* text = u8"A \u00C0\u3000\uAC00";    // A, (space), À, (wide space), 가
  auto chars = hanal::Char::characterize(text);
  EXPECT_EQ(5, chars.size());
  EXPECT_EQ(L'A', chars[0]->wchar);    // 'A' with length 1
  EXPECT_EQ(text, chars[0]->start);
  EXPECT_EQ(text + 1, chars[0]->end);
  EXPECT_EQ(L' ', chars[1]->wchar);    // ' ' with length 1
  EXPECT_EQ(text + 1, chars[1]->start);
  EXPECT_EQ(text + 2, chars[1]->end);
  EXPECT_EQ(L'\u00C0', chars[2]->wchar);    // 'À' with length 2
  EXPECT_EQ(text + 2, chars[2]->start);
  EXPECT_EQ(text + 4, chars[2]->end);
  EXPECT_EQ(L'\u3000', chars[3]->wchar);    // '　' with length 3
  EXPECT_EQ(text + 4, chars[3]->start);
  EXPECT_EQ(text + 7, chars[3]->end);
  EXPECT_EQ(L'\uAC00', chars[4]->wchar);    // '가' with length 3
  EXPECT_EQ(text + 7, chars[4]->start);
  EXPECT_EQ(text + 10, chars[4]->end);

  auto zero_chars = hanal::Char::characterize("");    // zero length text
  EXPECT_EQ(0, zero_chars.size());

  EXPECT_THROW(hanal::Char::characterize(nullptr), hanal::Except);    // null pointer
  EXPECT_THROW(hanal::Char::characterize("\xFE"), hanal::Except);    // invalid UTF-8 character
}


TEST_F(CharTest, is_space) {
  EXPECT_TRUE(hanal::Char(L' ', nullptr, nullptr).is_space());
  EXPECT_TRUE(hanal::Char(L'\t', nullptr, nullptr).is_space());
  EXPECT_TRUE(hanal::Char(L'\v', nullptr, nullptr).is_space());
  EXPECT_TRUE(hanal::Char(L'\r', nullptr, nullptr).is_space());
  EXPECT_TRUE(hanal::Char(L'\n', nullptr, nullptr).is_space());
  EXPECT_TRUE(hanal::Char(L'\u3000', nullptr, nullptr).is_space());    // (wide space)

  EXPECT_FALSE(hanal::Char(L'A', nullptr, nullptr).is_space());
  EXPECT_FALSE(hanal::Char(L'\0', nullptr, nullptr).is_space());
  EXPECT_FALSE(hanal::Char(L'\uAC00', nullptr, nullptr).is_space());    // '가'
}
