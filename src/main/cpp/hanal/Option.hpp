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
  explicit Option(std::string opt_str);    ///< ctor

  int word_merge();    ///< get word_merge option value
  bool anal_back();    ///< get anal_back option value

 private:
  int _word_merge = 1;    ///< word merge count. default: 1
  bool _anal_back = true;    ///< analyze backward. default: true
};


}    // namespace hanal


#endif  // HANAL_OPTION_HPP
