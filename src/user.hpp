#include <iostream>

#include "user_file_storage.hpp"

class user {
 public:
  const int rank;
  user(std::string &in_id, std::string &in_password);
  void add_user(std::string &in_id, std::string &in_password);

 private:
  std::string id;
  std::string password;
};

class customer : public user {
 public:
  const int rank;
  customer(std::string &in_id, std::string &in_password);

 private:
};

class faculty : public customer {
 public:
  const int rank;

  faculty(std::string &in_id, std::string &in_password);

 private:
};

class manager : public faculty {
 public:
  const int rank;

  manager(std::string &in_id, std::string &in_password);

 private:
};