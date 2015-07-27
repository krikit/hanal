/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/ViterbiTrellis.hpp"


//////////////
// includes //
//////////////
#include <vector>

#include "hanal/Word.hpp"


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
ViterbiTrellis::node_t::node_t(const SHDPTRVEC(Morph)& anal_result_)
    : anal_result(anal_result_) {
}


ViterbiTrellis::ViterbiTrellis(const SHDPTRVEC(Word)& words)
    : nodes(Word::char_len(words)) {
}


void ViterbiTrellis::add_node(const SHDPTRVEC(Morph)& anal_result, int idx, int len) {
  auto curr_node = std::make_shared<node_t>(anal_result);
  auto curr_node_vec = nodes[idx + len - 1];
  curr_node_vec.emplace_back(curr_node);
  if (idx > 0) {
    auto prev_node_vec = nodes[idx - 1];
    for (auto& prev_node : prev_node_vec) {
      prev_node->right_edges.emplace_back(curr_node);
      curr_node->left_edges.emplace_back(prev_node);
    }
  }
}


}    // namespace hanal
