/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */

#ifndef HANAL_TRANSMAT_HPP
#define HANAL_TRANSMAT_HPP


//////////////
// includes //
//////////////
#include "hanal/MappedDic.hpp"
#include "hanal/SejongTag.hpp"


namespace hanal {


/**
 * transition matrix
 */
class TransMat: public MappedDic<float> {
 public:
  /**
   * @brief  get transition weight from/to tag
   * @param  from  from tag
   * @param  to    to tag
   * @return       transition weight
   */
  float get(SejongTag from, SejongTag to);
};


}    // namespace hanal


#endif  // HANAL_TRANSMAT_HPP
