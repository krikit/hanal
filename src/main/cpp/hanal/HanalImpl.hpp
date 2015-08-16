/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_HANALIMPL_HPP
#define HANAL_HANALIMPL_HPP


//////////////
// includes //
//////////////
#include <list>
#include <mutex>    // NOLINT
#include <string>

#include "hanal/HanalApi.hpp"
#include "hanal/macro.hpp"


namespace hanal {


class MorphDic;
class Option;
class StateFeatDic;
class TransMat;


/**
 * implementation of hanal API
 */
class HanalImpl: public HanalApi {
 public:
  HanalImpl();    ///< ctor
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
  SHDPTR(Option) _option;    ///< option
  SHDPTR(MorphDic) _morph_dic;    ///< morpheme dictionary
  SHDPTR(StateFeatDic) _state_feat_dic;    ///< state-feature dictionary
  SHDPTR(TransMat) _trans_mat;    ///< transition matrix

  static const int _CACHE_MAX = 1000;    ///< max number of cache
  std::list<std::string> _str_buf;    ///< string buffer for caching
  const std::string& _cache(std::string str);    ///< cache string in internal buffer
};


}    // namespace hanal


#endif  // HANAL_HANALIMPL_HPP
