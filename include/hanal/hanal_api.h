/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef INCLUDE_HANAL_HANAL_API_H_
#define INCLUDE_HANAL_HANAL_API_H_


///////////////
// constants //
///////////////
#define HANAL_VERSION "0.1"


#ifdef __cplusplus
extern "C" {
#endif


///////////////
// functions //
///////////////
/**
 * @brief   get version string
 * @return  version string. it should be same to HANAL_VERSION
 */
const char* hanal_version();


/**
 * @brief           open resources
 * @param  rsc_dir  resource directory
 * @param  opt_str  option string
 * @return          handle. -1 if failed
 */
int hanal_open(const char* rsc_dir, const char* opt_str);


/**
 * @brief          close resources
 * @param  handle  handle got from open
 */
void hanal_close(int handle);


/**
 * @brief           part-of-speech tagging
 * @param  handle   handle got from open
 * @param  sent     input sentence
 * @param  opt_str  option string
 * @return          tagged result
 */
const char* hanal_pos_tag(int handle, const char* sent, const char* opt_str);


#ifdef __cplusplus
}    // __cplusplus
#endif


#endif  // INCLUDE_HANAL_HANAL_API_H_
