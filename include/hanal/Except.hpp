/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_EXCEPT_HPP
#define HANAL_EXCEPT_HPP


//////////////
// includes //
//////////////
#include <string>
#include <exception>


////////////
// macros //
////////////
#define EXCEPT(msg) hanal::Except(msg, __FILE__, __LINE__, __PRETTY_FUNCTION__)


namespace hanal {


/**
 * standard exception thrown by hanal module
 */
class Except: public std::exception {
 public:
  /**
   * @param  msg   error message
   * @param  file  source file (for debug)
   * @param  line  line number in source file (for debug)
   * @param  func  function name (for debug)
   */
  explicit Except(std::string msg, const char* file = nullptr, const int line = 0, const char* func = nullptr);
  virtual const char* what() const noexcept;
  std::string debug();    ///< message with some debug information

 private:
  std::string _msg;    ///< error message
  const char* _file = nullptr;    ///< source file
  const int _line = 0;    ///< line number in source file
  const char* _func = nullptr;    ///< function name
};


}    // namespace hanal


#endif  // HANAL_EXCEPT_HPP
