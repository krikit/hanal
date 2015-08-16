/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_VITERBITRELLIS_HPP
#define HANAL_VITERBITRELLIS_HPP


//////////////
// includes //
//////////////
#include <memory>
#include <string>
#include <vector>

#include "hanal/macro.hpp"


namespace hanal {


class Word;
class Morph;


/**
 * node of Viterbi trellis
 */
struct _trellis_node_t {
  SHDPTRVEC(_trellis_node_t) left_edges;
  SHDPTRVEC(_trellis_node_t) right_edges;
  const SHDPTRVEC(Morph)& anal_result;    ///< analysis result (vector of morphemes)
  explicit _trellis_node_t(const SHDPTRVEC(Morph)& anal_result_);    ///< ctor
  std::string str();    ///< get string for debugging
};


/**
 * Trellis of edges and nodes for Viterbi algorithm
 */
class ViterbiTrellis {
 public:
  /** @brief  node positions (same to length of non-space characters) */
  std::vector<SHDPTRVEC(_trellis_node_t)> nodes;

  explicit ViterbiTrellis(const SHDPTRVEC(Word)& words);    ///< ctor

  /**
   * @brief               add node with given analysis result into idx position
   * @param  anal_result  analysis result (vector of morphemes)
   * @param  idx          index of insert position
   * @param  len          character length
   */
  void add_node(const SHDPTRVEC(Morph)& anal_result, int idx, int len);

  std::string str();    ///< get string for debugging
};


}    // namespace hanal


#endif  // HANAL_VITERBITRELLIS_HPP
