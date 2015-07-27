/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#ifndef HANAL_MAPPEDDIC_HPP
#define HANAL_MAPPEDDIC_HPP


//////////////
// includes //
//////////////
#include <string>

#include "boost/iostreams/device/mapped_file.hpp"
#include "boost/lexical_cast.hpp"
#include "hanal/Except.hpp"


namespace hanal {


template<typename T>
class MappedDic {
 public:
  virtual ~MappedDic() {
    close();
  }

  /**
   * @brief              open resource file
   * @param  path        file path
   * @param  is_private  open mapped file as private mode
   */
  virtual void open(std::string path, bool is_private = false) {
    close();
    try {
      if (is_private) {
        _map_file.open(path, boost::iostreams::mapped_file::priv);
      } else {
        _map_file.open(path, boost::iostreams::mapped_file::readonly);
      }
    } catch (std::exception exc) {
      HANAL_THROW(exc.what());
    }
    HANAL_ASSERT(_map_file.is_open(), "Fail to open file: " + path);
    HANAL_ASSERT(_map_file.size() > 0 && (_map_file.size() % sizeof(T)) == 0,
                 "Invalid size of file: " + boost::lexical_cast<std::string>(_map_file.size()));
  }

  /**
   * @brief  close resource file
   */
  virtual void close() {
    _map_file.close();
  }

  /**
   * @brief  get read only data pointer
   */
  virtual const T* const_data() const {
    return reinterpret_cast<const T*>(_map_file.const_data());
  }

  /**
   * @brief  get data pointer
   */
  virtual T* data() const {
    return reinterpret_cast<T*>(_map_file.data());
  }

  /**
   * @brief   get number of data element
   * @return  number of element
   */
  virtual int size() const {
    return _map_file.size() / sizeof(T);
  }

 private:
  boost::iostreams::mapped_file _map_file;    ///< mmap file
};


}    // namespace hanal


#endif  // HANAL_MAPPEDDIC_HPP
