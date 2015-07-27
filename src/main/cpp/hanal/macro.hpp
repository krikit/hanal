/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_MACRO_HPP
#define HANAL_MACRO_HPP


//////////////
// includes //
//////////////
#include <memory>
#include <vector>


////////////
// macros //
////////////
#define SHDPTR(T) std::shared_ptr<T>
#define SHDPTRVEC(T) std::vector<std::shared_ptr<T>>


#endif  // HANAL_MACRO_HPP
