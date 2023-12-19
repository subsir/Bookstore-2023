#ifndef LOG_HPP
#define LOG_HPP
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

class Log {
 private:
  std::string filename;
  std::fstream file;

 public:
  Log(const std::string &filename) : filename(filename) {
    file.open(filename, std::ios::out | std::ios::app);
    file.close();
  }

  ~Log() {}

  void write(const std::string &message) {
    file.open(filename, std::ios::out | std::ios::app);
    file << message << std::endl;
    file.flush();
    file.close();
  }

  void read() {
    file.open(filename, std::ios::in | std::ios::out);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::cout << buffer.str();
    file.close();
  }
};
#endif