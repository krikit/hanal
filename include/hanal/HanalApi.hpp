/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_HANALAPI_HPP
#define HANAL_HANALAPI_HPP


//////////////
// includes //
//////////////
#include <memory>
#include <string>


namespace hanal {


/**
 * API class for hanal
 */
class HanalApi {
 public:
  /**
   * @brief   create API object
   * @return  API object
   */
  static std::shared_ptr<HanalApi> create();

  /**
   * @brief           open resources
   * @param  rsc_dir  resource directory
   * @param  opt_str  option string
   */
  virtual void open(std::string rsc_dir, std::string opt_str) = 0;

  virtual void close() = 0;    ///< close resources

  /**
   * @brief           part-of-speech tagging
   * @param  sent     input sentence
   * @param  opt_str  option string
   * @return          tagged result. JSON format
   */
  virtual const std::string& pos_tag(const char* sent, const char* opt_str) = 0;
};


}    // namespace hanal


#endif  // HANAL_HANALAPI_HPP
