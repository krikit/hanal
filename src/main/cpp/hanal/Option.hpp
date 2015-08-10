/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014 - 2015,
  krikit. All rights reserved. BSD 2 - Clause License
 */


#ifndef HANAL_OPTION_HPP
#define HANAL_OPTION_HPP


//////////////
// includes //
//////////////
#include <string>


namespace hanal {


/**
 * hanal option
 */
class Option {
 public:
  int word_merge = 1;    ///< word merge count. default: 1
  bool anal_back = true;    ///< analyze backward. default: true

  explicit Option(std::string opt_str);    ///< ctor

  /**
   * @brief           run-time override option
   * @param  opt_str  run-time option
   * @return          overrided option
   */
  Option override(std::string opt_str);
};


}    // namespace hanal


#endif  // HANAL_OPTION_HPP
