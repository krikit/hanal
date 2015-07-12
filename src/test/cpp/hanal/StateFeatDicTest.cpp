/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


//////////////
// includes //
//////////////
#include <map>
#include <string>

#include "boost/log/core.hpp"
#include "boost/log/expressions.hpp"
#include "boost/log/trivial.hpp"
#include "gtest/gtest.h"
#include "hanal/StateFeatDic.hpp"


extern std::map<std::string, std::string> prog_args;    // arguments passed to main program


/**
 * test fixture for StateFeatDic
 */
class StateFeatDicTest: public testing::Test {
 protected:
  virtual void SetUp() {
    auto iter = prog_args.find("rsc-dir");
    if (iter == prog_args.end()) FAIL() << "--rsc-dir argument required";
    rsc_dir = prog_args["rsc-dir"];
    ASSERT_NO_THROW(state_feat_dic.open(rsc_dir)) << "rsc_dir: " << rsc_dir;
  }

  hanal::StateFeatDic state_feat_dic;
  std::string rsc_dir;    ///< resource directory
};


TEST_F(StateFeatDicTest, open_close) {
  hanal::StateFeatDic state_feat_dic;
  EXPECT_NO_THROW(state_feat_dic.open(rsc_dir)) << "rsc_dir: " << rsc_dir;
  EXPECT_NO_THROW(state_feat_dic.close());

  EXPECT_THROW(state_feat_dic.open(rsc_dir + "/state_feat_dic.trie"), hanal::Except);    // file, not directory
  EXPECT_THROW(state_feat_dic.open(rsc_dir + "/.."), hanal::Except);    // invalid directory
  EXPECT_THROW(state_feat_dic.open(rsc_dir + "/__not_existing_dir__"), hanal::Except);
}


TEST_F(StateFeatDicTest, get) {
  // high weighted state-features
  EXPECT_LT(1.0, state_feat_dic.get(hanal::SejongTag::SF, L"S_0=."));
  EXPECT_LT(1.0, state_feat_dic.get(hanal::SejongTag::SF, L"S_0=?"));
  EXPECT_LT(1.0, state_feat_dic.get(hanal::SejongTag::SF, L"S_0=!"));

  // low weighted state-features
  EXPECT_GT(-1.0, state_feat_dic.get(hanal::SejongTag::XSV, L"LSP"));
  EXPECT_GT(-1.0, state_feat_dic.get(hanal::SejongTag::VX, L"BOS"));

  // non existing cases
  EXPECT_EQ(0.0, state_feat_dic.get(hanal::SejongTag::NNG, L"__non_existing_feature__"));
  EXPECT_EQ(0.0, state_feat_dic.get(static_cast<hanal::SejongTag>(99), L"L_0='"));
}
