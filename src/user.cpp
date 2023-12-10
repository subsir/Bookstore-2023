#include "user.hpp"

user::user(std::string &in_id, std::string &in_password)
    : rank(0), id(in_id), password(in_password) {}

customer::customer(std::string &in_id, std::string &in_password)
    : user(in_id, in_password), rank(1) {}
faculty::faculty(std::string &in_id, std::string &in_password)
    : customer(in_id, in_password), rank(3) {}
manager::manager(std::string &in_id, std::string &in_password)
    : faculty(in_id, in_password), rank(7) {}