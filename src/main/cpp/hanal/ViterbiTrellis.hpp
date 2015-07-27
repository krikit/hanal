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
#include <vector>

#include "hanal/macro.hpp"


namespace hanal {


class Word;
class Morph;


/**
 * Trellis of edges and nodes for Viterbi algorithm
 */
class ViterbiTrellis {
 public:
  struct node_t {
    SHDPTRVEC(node_t) left_edges;
    SHDPTRVEC(node_t) right_edges;
    const SHDPTRVEC(Morph)& anal_result;    ///< analysis result (vector of morphemes)
    explicit node_t(const SHDPTRVEC(Morph)& anal_result_);    ///< ctor
  };

  /** @brief  node positions (same to length of non-space characters) */
  std::vector<SHDPTRVEC(node_t)> nodes;

  explicit ViterbiTrellis(const SHDPTRVEC(Word)& words);    ///< ctor

  void add_node(const SHDPTRVEC(Morph)& anal_result, int idx, int len);
};


}    // namespace hanal


#endif  // HANAL_VITERBITRELLIS_HPP
