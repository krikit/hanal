/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/ViterbiTrellis.hpp"


//////////////
// includes //
//////////////
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "hanal/Morph.hpp"
#include "hanal/Word.hpp"


namespace hanal {


////////////////////
// ctors and dtor //
////////////////////
_trellis_node_t::_trellis_node_t(const SHDPTRVEC(Morph)& anal_result_)
    : anal_result(anal_result_) {
}


std::string _trellis_node_t::str() {
  std::ostringstream oss;
  oss << Morph::str(anal_result) << std::endl;
  if (left_edges.size() > 0) {
    oss << "    [LEFT]  ";
    for (auto& edge : left_edges) {
      oss << Morph::str(edge->anal_result);
      if (edge != left_edges.back()) oss << " || ";
    }
  }
  if (left_edges.size() > 0 && right_edges.size() > 0) oss << std::endl;
  if (right_edges.size() > 0) {
    oss << "    [RIGHT] ";
    for (auto& edge : right_edges) {
      oss << Morph::str(edge->anal_result);
      if (edge != right_edges.back()) oss << " || ";
    }
  }
  return oss.str();
}


ViterbiTrellis::ViterbiTrellis(const SHDPTRVEC(Word)& words)
    : nodes(Word::char_len(words)) {
}


void ViterbiTrellis::add_node(const SHDPTRVEC(Morph)& anal_result, int idx, int len) {
  auto curr_node = std::make_shared<_trellis_node_t>(anal_result);
  auto& curr_node_vec = nodes[idx + len - 1];
  curr_node_vec.emplace_back(curr_node);
  if (idx > 0) {
    auto& prev_node_vec = nodes[idx - 1];
    for (auto& prev_node : prev_node_vec) {
      prev_node->right_edges.emplace_back(curr_node);
      curr_node->left_edges.emplace_back(prev_node);
    }
  }
}


std::string ViterbiTrellis::str() {
  std::ostringstream oss;

  for (int idx = 0; idx < nodes.size(); ++idx) {
    auto& nodes_idx = nodes[idx];
    oss << "[" << idx << "] '" << "'" << std::endl;
    for (int jdx = 0; jdx < nodes_idx.size(); ++jdx) {
      auto& node = nodes_idx[jdx];
      oss << "  [" << jdx << "] " << node->str() << std::endl;
    }
  }

  return oss.str();
}


}    // namespace hanal
