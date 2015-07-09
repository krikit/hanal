/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


//////////////
// includes //
//////////////
#include <map>
#include <string>

#include "gtest/gtest.h"
#include "hanal/MorphDic.hpp"


extern std::map<std::string, std::string> prog_args;    // arguments passed to main program


/**
 * test fixture for MorphDic
 */
class MorphDicTest: public testing::Test {
 protected:
  virtual void SetUp() {
    auto iter = prog_args.find("rsc-dir");
    if (iter == prog_args.end()) FAIL() << "--rsc-dir argument required";
    rsc_dir = prog_args["rsc-dir"];
  }

  std::string rsc_dir;    ///< resource directory
};


TEST_F(MorphDicTest, open_close) {
  hanal::MorphDic morph_dic;
  EXPECT_NO_THROW(morph_dic.open(rsc_dir)) << "rsc_dir: " << rsc_dir;
  EXPECT_NO_THROW(morph_dic.close());

  EXPECT_THROW(morph_dic.open(rsc_dir + "/morph.trie"), hanal::Except);    // file, not directory
  EXPECT_THROW(morph_dic.open(rsc_dir + "/.."), hanal::Except);    // invalid directory
  EXPECT_THROW(morph_dic.open(rsc_dir + "/__not_existing_dir__"), hanal::Except);
}

