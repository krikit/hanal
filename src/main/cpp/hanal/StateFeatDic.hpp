/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_STATEFEATDIC_HPP
#define HANAL_STATEFEATDIC_HPP


//////////////
// includes //
//////////////
#include <string>
#include <vector>

#include "hanal/MappedDic.hpp"
#include "hanal/SejongTag.hpp"
#include "hanal/Trie.hpp"


namespace hanal {

/**
 * state-features dictionary
 */
class StateFeatDic {
 public:
  virtual ~StateFeatDic();    ///< dtor

  /**
   * @brief           open resources
   * @param  rsc_dir  resource directory
   */
  void open(std::string rsc_dir);

  void close();    ///< close resources

  /**
   * @brief         get state-feature weight
   * @param  state  state
   * @param  feat   feature
   * @return        weight
   */
  float get(SejongTag state, const wchar_t* feat);

 private:
  Trie _trie;    ///< key trie
  MappedDic<float> _value;    ///< values (state-feature weights)
};


}    // namespace hanal


#endif  // HANAL_STATEFEATDIC_HPP
