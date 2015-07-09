/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_HANALIMPL_HPP
#define HANAL_HANALIMPL_HPP


//////////////
// includes //
//////////////
#include <mutex>    // NOLINT
#include <string>

#include "hanal/HanalApi.hpp"


namespace hanal {


/**
 * implementation of hanal API
 */
class HanalImpl: public HanalApi {
 public:
  virtual ~HanalImpl();    ///< dtor

  /**
   * @brief           open resources
   * @param  rsc_dir  resource directory
   * @param  opt_str  option string
   */
  void open(std::string rsc_dir, std::string opt_str);

  void close();    ///< close resources

  /**
   * @brief           part-of-speech tagging
   * @param  sent     input sentence
   * @param  opt_str  option string
   * @return          tagged result
   */
  const std::string& pos_tag(const char* sent, const char* opt_str);

 private:
  std::recursive_mutex _mutex;    ///< mutex to access API methods exclusively
};


}    // namespace hanal


#endif  // HANAL_HANALIMPL_HPP
