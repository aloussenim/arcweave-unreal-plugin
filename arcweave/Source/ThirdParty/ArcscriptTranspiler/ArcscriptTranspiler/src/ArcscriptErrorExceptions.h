#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <exception>

namespace Arcweave {
  class RuntimeErrorException : public std::exception {
    public:
    std::string message;
    int line = -1;
    int charPositionInLine = -1;
    RuntimeErrorException(std::string msg) {
      message = msg;
    };
    RuntimeErrorException(std::string msg, int _line, size_t _charPositionInLine) {
      line = _line;
      charPositionInLine = _charPositionInLine;
      
      if (line > -1) {
          std::ostringstream oss;
          oss << "line " << line << ":" << charPositionInLine << " " << message << std::endl;
          message = oss.str(); // Get the string
      }
      else {
          message = msg;
      }
    };
    char const* what() const noexcept override {
      return message.c_str();
    }
  };

  class ParseErrorException : public std::exception {
    public:
    std::string message;
    int line = -1;
    int charPositionInLine = -1;
    ParseErrorException(std::string msg) {
      message = msg;
    };
    ParseErrorException(std::string msg, int _line, size_t _charPositionInLine) {
      line = _line;
      if (line > -1) {
          std::ostringstream oss;
          oss << "line " << line << ":" << charPositionInLine << " " << message << std::endl;
          message = oss.str(); // Get the string
      }
      else {
          message = msg;
      }
      charPositionInLine = _charPositionInLine;
    };
    char const* what() const noexcept override {
      return message.c_str();
    }
  };
}
