#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <string>

class Exception: public std::exception
{
  public:
	Exception() {}
	Exception(const std::string & message): message_(message) {}
	Exception(const std::exception & e): message_(e.what()) {}
	Exception(const Exception & e): message_(e.message_) {}
	virtual ~Exception() throw() {}
	
	const char * what() const throw() { return message_.c_str(); }
	
  private:
	std::string message_;
};

#endif