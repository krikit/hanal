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
  auto found = trie.find(L"a");
  EXPECT_TRUE(found != boost::none);
  BOOST_LOG_TRIVIAL(debug) << "a: " << *found;
  EXPECT_TRUE(*trie.find(L"a") == *trie.find(std::wstring(L"a")));

  found = trie.find(L"__not_found_key__");
  EXPECT_TRUE(found == boost::none);

  EXPECT_THROW(trie.find(nullptr), hanal::Except);
  EXPECT_THROW(trie.find(L""), hanal::Except);
}
