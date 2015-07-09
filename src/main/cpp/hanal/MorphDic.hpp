/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_MORPHDIC_HPP
#define HANAL_MORPHDIC_HPP


//////////////
// includes //
//////////////
#include <string>
#include <vector>

#include "hanal/MappedDic.hpp"
#include "hanal/Trie.hpp"


namespace hanal {

/**
 * morpheme dictionary
 */
class MorphDic {
 public:
  virtual ~MorphDic();    ///< dtor

  /**
   * @brief           open resources
   * @param  rsc_dir  resource directory
   */
  void open(std::string rsc_dir);

  void close();    ///< close resources

 private:
  Trie _trie;    ///< syllable trie
  MappedDic<wchar_t> _value;    ///< analysis results (morphemes)
  std::vector<const wchar_t*> _val_idx;    ///< index for value (analysis results text)
};


}    // namespace hanal


#endif  // HANAL_MORPHDIC_HPP
