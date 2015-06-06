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
  }
};


TEST_F(TrieTest, open_close) {
  hanal::Trie trie;
  std::string path(prog_args["rsc-dir"] + "/main_dic.trie");
  EXPECT_NO_THROW(trie.open(path)) << "path: " << path;
  EXPECT_NO_THROW(trie.close());

  EXPECT_THROW(trie.open(prog_args["rsc-dir"]), hanal::Except);    // directory, not file
  EXPECT_THROW(trie.open(prog_args["rsc-dir"] + "__not_existing_file__"), hanal::Except);
}
