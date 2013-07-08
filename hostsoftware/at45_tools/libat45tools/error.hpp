#ifndef LIBAT45TOOLS_ERROR_HPP
#define LIBAT45TOOLS_ERROR_HPP 1





#include <string>
#include <exception>
#include <tr1/memory>
#include <boost/system/error_code.hpp>

namespace at45tools {
  class GenericException : public std::exception {
    public:
      typedef std::tr1::shared_ptr<GenericException> Ptr;
      GenericException (const std::string reason) : _reason(reason) {};
      virtual ~GenericException() throw() {};
      virtual const char* what() const throw() { return reason().c_str(); }
      virtual const std::string& reason() const { return _reason; }

    private:
      std::string _reason;
  };

  class IOException : public GenericException {
    public:
      typedef std::tr1::shared_ptr<IOException> Ptr;
      IOException (const std::string reason) :
        at45tools::GenericException(reason) {};
      virtual ~IOException() throw() {};
  };

}

#endif /* LIBAT45TOOLS_ERROR_HPP */
