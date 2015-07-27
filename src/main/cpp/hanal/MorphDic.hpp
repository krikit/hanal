/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_MORPHDIC_HPP
#define HANAL_MORPHDIC_HPP


//////////////
// includes //
//////////////
#include <list>
#include <string>
#include <vector>

#include "hanal/MappedDic.hpp"
#include "hanal/Morph.hpp"
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

  /**
   * @brief        lookup morpheme dictionary
   * @param  text  text to search
   * @return       all matches
   */
  std::list<Trie::match_t> lookup(const wchar_t* text) const;

  /**
   * @brief       get value (analysis result)
   * @param  idx  value index
   * @return      value
   */
  const std::vector<SHDPTRVEC(Morph)>& value(int idx);

 private:
  Trie _trie;    ///< syllable trie
  MappedDic<wchar_t> _value;    ///< raw value of analysis results (vector of morphemes)
  std::vector<wchar_t*> _val_idx;    ///< string index for raw value
  /** @brief  parsed value (analysis results) cache */
  std::vector<std::vector<SHDPTRVEC(Morph)>> _val_cache;
};


}    // namespace hanal


#endif  // HANAL_MORPHDIC_HPP
