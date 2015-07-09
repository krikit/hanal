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
#include "hanal/Except.hpp"
#include "hanal/TransMat.hpp"


extern std::map<std::string, std::string> prog_args;    // arguments passed to main program


/**
 * test fixture for TransMat
 */
class TransMatTest: public testing::Test {
 protected:
  virtual void SetUp() {
    auto iter = prog_args.find("rsc-dir");
    if (iter == prog_args.end()) FAIL() << "--rsc-dir argument required";
    trans_mat_path = prog_args["rsc-dir"] + "/trans_mat.bin";
    ASSERT_NO_THROW(trans_mat.open(trans_mat_path)) << "trans_mat_path: " << trans_mat_path;
  }

  hanal::TransMat trans_mat;
  std::string trans_mat_path;    ///< path for trans_mat
};


TEST_F(TransMatTest, open_close) {
  hanal::TransMat tans_mat;
  EXPECT_NO_THROW(trans_mat.open(trans_mat_path)) << "trans_mat_path: " << trans_mat_path;
  EXPECT_NO_THROW(trans_mat.close());

  EXPECT_THROW(trans_mat.open(prog_args["rsc-dir"]), hanal::Except);    // directory, not file
  EXPECT_THROW(trans_mat.open(prog_args["rsc-dir"] + "__not_existing_file__"), hanal::Except);
}


TEST_F(TransMatTest, get) {
  // frequent cases of transitions
  EXPECT_LT(1.0, trans_mat.get(hanal::SejongTag::NNG, hanal::SejongTag::XSV));
  EXPECT_LT(1.0, trans_mat.get(hanal::SejongTag::VV, hanal::SejongTag::EC));

  // rare cases of transitions
  EXPECT_GT(-1.0, trans_mat.get(hanal::SejongTag::JKQ, hanal::SejongTag::VX));
  EXPECT_GT(-1.0, trans_mat.get(hanal::SejongTag::MAG, hanal::SejongTag::JKS));

  // exception cases
  EXPECT_THROW(trans_mat.get(static_cast<hanal::SejongTag>(9999), hanal::SejongTag::NNG), hanal::Except);
  EXPECT_THROW(trans_mat.get(static_cast<hanal::SejongTag>(-9999), hanal::SejongTag::NNG), hanal::Except);
  EXPECT_THROW(trans_mat.get(hanal::SejongTag::VV, static_cast<hanal::SejongTag>(-9999)), hanal::Except);
  EXPECT_THROW(trans_mat.get(hanal::SejongTag::VV, static_cast<hanal::SejongTag>(9999)), hanal::Except);
}
