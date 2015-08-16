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
#include "hanal/hanal_api.h"


extern std::map<std::string, std::string> prog_args;    // arguments passed to main program


TEST(hanal_api, hanal_version) {
  EXPECT_STREQ(HANAL_VERSION, hanal_version());
}


TEST(hanal_api, hanal_open_close) {
  std::string rsc_dir = prog_args["rsc-dir"];

  int handle = hanal_open(rsc_dir.c_str(), "");
  EXPECT_LE(0, handle) << "rsc_dir: " << rsc_dir;
  EXPECT_NO_THROW(hanal_close(handle));

  EXPECT_GT(0, hanal_open((rsc_dir + "/morph.trie").c_str(), ""));    // file, not directory
  EXPECT_GT(0, hanal_open((rsc_dir + "/..").c_str(), ""));    // invalid directory
  EXPECT_GT(0, hanal_open((rsc_dir + "/__not_existing_dir__").c_str(), ""));
  EXPECT_GT(0, hanal_open(nullptr, ""));
}


TEST(hanal_api, hanal_pos_tag) {
  std::string rsc_dir = prog_args["rsc-dir"];
  int handle = hanal_open(rsc_dir.c_str(), "");

  const char* sent1 = u8"아버지 가방에들어 가신다.";
  const char* result1 = hanal_pos_tag(handle, sent1, "");
  std::cerr << result1 << std::endl;

  hanal_close(handle);
}
