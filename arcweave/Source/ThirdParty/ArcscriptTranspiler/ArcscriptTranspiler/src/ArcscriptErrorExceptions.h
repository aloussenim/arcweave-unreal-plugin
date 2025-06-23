#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <exception>

namespace Arcweave {
  class RuntimeErrorException : public std::exception {
    public:
    std::string message;
    size_t line;
    size_t charPositionInLine;
    RuntimeErrorException(std::string msg) {
      message = msg;
    };
    RuntimeErrorException(std::string msg, size_t _line, size_t _charPositionInLine) {
      message = msg;
      line = _line;
      charPositionInLine = _charPositionInLine;
    };
    char const* what() const noexcept override {
        if (line > -1) {
            std::ostringstream oss;
            oss << "line " << line << ":" << charPositionInLine << " " << message << std::endl;
            std::string temp_str = oss.str(); // Get the string
            
            // Determine the size needed (including null terminator)
            size_t size = temp_str.length() + 1;
            // Dynamically allocate memory for the string
            char* buffer = new char[size];
            strcpy(buffer, temp_str.c_str());
            return buffer;
        }
      return message.c_str();
    }
  };

  class ParseErrorException : public std::exception {
    public:
    std::string message;
    size_t line;
    size_t charPositionInLine;
    ParseErrorException(std::string msg) {
      message = msg;
    };
    ParseErrorException(std::string msg, size_t _line, size_t _charPositionInLine) {
      message = msg;
      line = _line;
      charPositionInLine = _charPositionInLine;
    };
    char const* what() const noexcept override {
      if (line > -1) {
        std::ostringstream oss;
        oss << "line " << line << ":" << charPositionInLine << " " << message << std::endl;
        std::string temp_str = oss.str(); // Get the string

        // Determine the size needed (including null terminator)
        size_t size = temp_str.length() + 1;
        // Dynamically allocate memory for the string
        char* buffer = new char[size];
        strcpy(buffer, temp_str.c_str());
        return buffer;
      }
      return message.c_str();
    }
  };
}
