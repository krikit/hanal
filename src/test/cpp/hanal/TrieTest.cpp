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
    morph_trie_path = prog_args["rsc-dir"] + "/morph.trie";
    state_feat_trie_path = prog_args["rsc-dir"] + "/state_feat.trie";
    ASSERT_NO_THROW(morph_trie.open(morph_trie_path)) << "morph_trie_path: " << morph_trie_path;
    ASSERT_NO_THROW(state_feat_trie.open(state_feat_trie_path)) << "state_feat_trie_path: " << state_feat_trie_path;
  }

  hanal::Trie morph_trie;
  hanal::Trie state_feat_trie;
  std::string morph_trie_path;    ///< path for morphemes morph_trie
  std::string state_feat_trie_path;    ///< path for state-features morph_trie
};


TEST_F(TrieTest, open_close) {
  hanal::Trie morph_trie;
  EXPECT_NO_THROW(morph_trie.open(morph_trie_path)) << "morph_trie_path: " << morph_trie_path;
  EXPECT_NO_THROW(morph_trie.close());

  hanal::Trie state_feat_trie;
  EXPECT_NO_THROW(state_feat_trie.open(state_feat_trie_path)) << "state_feat_trie_path: " << state_feat_trie_path;
  EXPECT_NO_THROW(state_feat_trie.close());

  EXPECT_THROW(morph_trie.open(prog_args["rsc-dir"]), hanal::Except);    // directory, not file
  EXPECT_THROW(state_feat_trie.open(prog_args["rsc-dir"] + "__not_existing_file__"), hanal::Except);
}


TEST_F(TrieTest, find) {
  auto morph_key = L"acceleration";
  auto morph_found = morph_trie.find(morph_key);
  EXPECT_TRUE(morph_found != boost::none);
  if (morph_found) {
    BOOST_LOG_TRIVIAL(debug) << morph_key << ": " << *morph_found;
    EXPECT_EQ(*morph_trie.find(morph_key), *morph_trie.find(std::wstring(morph_key)));
  }

  // 'BOS' feature at state 'NNG'. index of 'NNG' is 21, so state prefix 'A'+21 is 'V'
  auto state_feat_key = L"VBOS";
  auto state_feat_found = state_feat_trie.find(state_feat_key);
  EXPECT_TRUE(state_feat_found != boost::none);
  if (state_feat_found) {
    BOOST_LOG_TRIVIAL(debug) << state_feat_key << ": " << *state_feat_found;
    EXPECT_EQ(*state_feat_trie.find(state_feat_key), *state_feat_trie.find(std::wstring(state_feat_key)));
  }

  morph_found = morph_trie.find(L"__not_found_key__");
  EXPECT_TRUE(morph_found == boost::none);
  state_feat_found = state_feat_trie.find(L"");
  EXPECT_TRUE(state_feat_found == boost::none);

  EXPECT_THROW(morph_trie.find(nullptr), hanal::Except);
  EXPECT_THROW(state_feat_trie.find(nullptr), hanal::Except);
}


TEST_F(TrieTest, search_all) {
  auto text = L"accelerations";
  auto matches = morph_trie.search_common_prefix_matches(text);
  EXPECT_EQ(5, matches.size());
  if (5 == matches.size()) {
    BOOST_LOG_TRIVIAL(debug) << text << ": " << matches.size() << " entries";
    EXPECT_EQ(morph_trie.search_common_prefix_matches(text).size(),
              morph_trie.search_common_prefix_matches(std::wstring(text)).size());
  }

  matches = morph_trie.search_common_prefix_matches(L"뷁");
  EXPECT_EQ(0, matches.size());
  matches = morph_trie.search_common_prefix_matches(L"");
  EXPECT_EQ(0, matches.size());

  EXPECT_THROW(morph_trie.search_common_prefix_matches(nullptr), hanal::Except);
}
