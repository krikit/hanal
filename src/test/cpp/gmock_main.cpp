// Copyright 2008, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: wan@google.com (Zhanyong Wan)

#include <map>
#include <iostream>
#include <string>

#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include "boost/log/core.hpp"
#include "boost/log/expressions.hpp"
#include "boost/log/trivial.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

// global variable for program arguments
std::map<std::string, std::string> prog_args;

// MS C++ compiler/linker has a bug on Windows (not on Windows CE), which
// causes a link error when _tmain is defined in a static library and UNICODE
// is enabled. For this reason instead of _tmain, main function is used on
// Windows. See the following link to track the current status of this bug:
// http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=394464  // NOLINT
#if GTEST_OS_WINDOWS_MOBILE
# include <tchar.h>  // NOLINT

GTEST_API_ int _tmain(int argc, TCHAR** argv) {
#else
GTEST_API_ int main(int argc, char** argv) {
#endif  // GTEST_OS_WINDOWS_MOBILE
  std::cout << "Running main() from gmock_main.cc\n";
  // Since Google Mock depends on Google Test, InitGoogleMock() is
  // also responsible for initializing Google Test.  Therefore there's
  // no need for calling testing::InitGoogleTest() separately.
  testing::InitGoogleMock(&argc, argv);

  boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);

  // parse program arguments
  for (int i = 1; i < argc; ++i) {
    // every arguments starts with '--' and splited into key and value with delimiter '='
    // for example: '--rsc-dir=../rsc' will be parsed into 'rsc-dir' for key and '../rsc' for value
    if (boost::starts_with(argv[i], "--")) {
      BOOST_LOG_TRIVIAL(debug) << "argv[" << i << "]: " << argv[i];
      char* delim_pos = strchr(&argv[i][2], '=');
      if (delim_pos > &argv[i][2]) {
        std::string key(&argv[i][2], (delim_pos - &argv[i][2]));
        boost::trim(key);
        std::string val(delim_pos + 1);
        boost::trim(val);
        if (!key.empty() && !val.empty()) {
          BOOST_LOG_TRIVIAL(debug) << "prog_args[" << key << "] = " << val;
          prog_args[key] = val;
        }
      }
    }
  }

  return RUN_ALL_TESTS();
}
