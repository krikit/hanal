/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


//////////////
// includes //
//////////////
#include <map>
#include <iostream>
#include <string>

#include "boost/log/trivial.hpp"
#include "gtest/gtest.h"
#include "hanal/Except.hpp"

#include "hanal/Trie.hpp"


extern std::map<std::string, std::string> prog_args;    // arguments passed to main program


/**
 * test fixture for Trie
 */
class TrieTest: public testing::Test {
 protected:
  virtual void SetUp() {
    auto iter = prog_args.find("rsc-dir");
    if (iter == prog_args.end()) FAIL() << "--rsc-dir argument required";
    trie_path = prog_args["rsc-dir"] + "/main_dic.trie";
    ASSERT_NO_THROW(trie.open(trie_path)) << "trie_path: " << trie_path;
  }

  hanal::Trie trie;
  std::string trie_path;    ///< path for trie
};


TEST_F(TrieTest, open_close) {
  hanal::Trie trie;
  EXPECT_NO_THROW(trie.open(trie_path)) << "trie_path: " << trie_path;
  EXPECT_NO_THROW(trie.close());

  EXPECT_THROW(trie.open(prog_args["rsc-dir"]), hanal::Except);    // directory, not file
  EXPECT_THROW(trie.open(prog_args["rsc-dir"] + "__not_existing_file__"), hanal::Except);
}


TEST_F(TrieTest, find) {
  auto key = L"acceleration";
  auto found = trie.find(key);
  EXPECT_TRUE(found != boost::none);
  if (found) {
    BOOST_LOG_TRIVIAL(debug) << key << ": " << *found;
    EXPECT_EQ(*trie.find(key), *trie.find(std::wstring(key)));
  }

  found = trie.find(L"__not_found_key__");
  EXPECT_TRUE(found == boost::none);
  found = trie.find(L"");
  EXPECT_TRUE(found == boost::none);

  EXPECT_THROW(trie.find(nullptr), hanal::Except);
}


TEST_F(TrieTest, search_all) {
  auto text = L"accelerations";
  auto matches = trie.search_common_prefix_matches(text);
  EXPECT_EQ(5, matches.size());
  if (5 == matches.size()) {
    BOOST_LOG_TRIVIAL(debug) << text << ": " << matches.size() << " entries";
    EXPECT_EQ(trie.search_common_prefix_matches(text).size(),
              trie.search_common_prefix_matches(std::wstring(text)).size());
  }

  matches = trie.search_common_prefix_matches(L"뷁");
  EXPECT_EQ(0, matches.size());
  matches = trie.search_common_prefix_matches(L"");
  EXPECT_EQ(0, matches.size());

  EXPECT_THROW(trie.search_common_prefix_matches(nullptr), hanal::Except);
}
