#pragma once
#include <string>
#include <exception>



class SkypeException : public std::exception {
  std::string m_Message;
 public:
  SkypeException(const std::string& msg) {
    m_Message = msg;
  }

  ~SkypeException() throw() {}

 public:
  const char* what() const throw() {
    return m_Message.c_str();
  }
};


class LoginException : public SkypeException {
 public: LoginException() : SkypeException("Login Failed.") {}
};

#include "SkypeEx.h"
