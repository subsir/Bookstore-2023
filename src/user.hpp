#ifndef user_h
#define user_h

#include <iostream>

#include "user_file_storage.hpp"

class user {
 public:
  int rank;
  std::string id;
  user(std::string &in_id, std::string &in_password)
      : id(in_id), password(in_password) {
    rank = 0;
  }

 private:
  std::string password;
};

class customer : public user {
 public:
  customer(std::string &in_id, std::string &in_password)
      : user(in_id, in_password) {
    rank = 1;
  }

 private:
};

class faculty : public customer {
 public:
  faculty(std::string &in_id, std::string &in_password)
      : customer(in_id, in_password) {
    rank = 3;
  }

 private:
};

class manager : public faculty {
 public:
  manager(std::string &in_id, std::string &in_password)
      : faculty(in_id, in_password) {
    rank = 7;
  }

 private:
};

#endif