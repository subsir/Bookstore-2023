#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "user.hpp"

std::vector<std::shared_ptr<user>> users;
UserfileDatabase user_db("user_file.txt");

bool valid_check(int type, std::string &input) {
  if (input.size() > 30) {
    return false;
  }
  if (type ==
      0) {  // 0代表 [UserID], [Password], [CurrentPassword], [NewPassword]
    for (int i = 0; i < input.size(); i++) {
      if ((input[i] < '0' or input[i] > '9') and
          (input[i] < 'a' or input[i] > 'z') and
          (input[i] < 'A' or input[i] > 'Z') and
          (input[i] < 'a' or input[i] > 'z') and
          (input[i] < 'a' or input[i] > 'z') and input[i] != '_') {
        return false;
      }
    }
    return true;
  }
  if (type == 1) {  // 1代表[Username]
    for (int i = 0; i < input.size(); i++) {
      if (input[i] < 32 or input[i] > 126) {
        return false;
      }
    }
    return true;
  }
  if (type == 2) {  // 2代表[Privilege]
    if (input.size() != 1) {
      return false;
    }
    if (input[0] != '1' and input[0] != '3' and input[0] != '7') {
      return false;
    }
    return true;
  }
  return false;
}

void handleInput(std::string &input) {
  if (input == "exit" or input == "quit") {
    exit(0);
  } else if (input == "su") {
    getline(std::cin, input);
    std::istringstream stream(input);
    std::string id, password, left;
    stream >> id >> password >> left;
    if (left != "") {
      std::cout << "Invalid\n";
      return;
    }
    int pos = user_db.index_find(id);
    bool login = false;
    int temp_priority = 0;
    if (pos != -1) {
      temp_priority = user_db.get_priority(pos);
      if (users.empty() != true and temp_priority > users.back()->rank) {
        login = true;
      }
      if (password == user_db.check(pos)) {
        login = true;
      }
    }
    if (login) {
      if (temp_priority == 1) {
        std::shared_ptr<user> ptr = std::make_shared<customer>(id, password);
        users.push_back(ptr);
      } else if (temp_priority == 3) {
        std::shared_ptr<user> ptr = std::make_shared<faculty>(id, password);
        users.push_back(ptr);
      } else if (temp_priority == 7) {
        std::shared_ptr<user> ptr = std::make_shared<manager>(id, password);
        users.push_back(ptr);
      }
    } else {
      std::cout << "Invalid" << std::endl;
    }
  } else if (input == "logout") {
    if (users.empty()) {
      std::cout << "Invalid" << std::endl;
    } else {
      users.pop_back();
    }
  } else if (input == "register") {
    getline(std::cin, input);
    std::istringstream stream(input);
    std::string id, password, username, left;
    stream >> id >> password >> username >> left;
    if (left != "" or id == "" or password == "" or username == "") {
      std::cout << "Invalid\n";
      return;
    }
    if (valid_check(0, id) and valid_check(0, password) and
        valid_check(1, username)) {
      if (user_db.index_find(id) == -1) {
        user_db.insert(id, username, password, 1);
      } else {
        std::cout << "Invalid\n";
      }
    } else {
      std::cout << "Invalid\n";
    }
  } else if (input == "delete") {
    getline(std::cin, input);
    if (users.empty() or users.back()->rank != 7) {
      std::cout << "Invalid\n";
      return;
    }
    std::istringstream stream(input);
    std::string id, left;
    stream >> id >> left;
    if (left != "") {
      std::cout << "Invalid\n";
      return;
    }
    bool login = false;
    for (auto it : users) {
      if (it->id == id) {
        login = true;
        break;
      }
    }
    if (login == false) {
      int pos = user_db.index_find(id);
      if (pos != -1) {
        user_db.remove(id);
      } else {
        std::cout << "Invalid\n";
      }
    } else {
      std::cout << "Invalid\n";
    }
  } else if (input == "useradd") {
    getline(std::cin, input);
    if (users.empty() or users.back()->rank < 3) {
      std::cout << "Invalid\n";
      return;
    }
    std::istringstream stream(input);
    std::string id, password, username, privilege, left;
    stream >> id >> password >> privilege >> username >> left;
    if (left != "" or id == "" or password == "" or username == "" or
        privilege == "") {
      std::cout << "Invalid\n";
      return;
    }
    if (valid_check(0, id) and valid_check(0, password) and
        valid_check(1, username) and valid_check(2, privilege)) {
      if (users.empty() or users.back()->rank <= (privilege[0] - '0')) {
        std::cout << "Invalid\n";
        return;
      }
      if (user_db.index_find(id) == -1) {
        user_db.insert(id, username, password, (privilege[0] - '0'));
      } else {
        std::cout << "Invalid\n";
      }
    } else {
      std::cout << "Invalid\n";
    }
  } else if (input == "passwd") {
    getline(std::cin, input);
    std::istringstream stream(input);
    std::string id, current_password, new_password, left;
    stream >> id >> current_password >> new_password >> left;
    if (left != "") {
      std::cout << "Invalid\n";
      return;
    }
    if (new_password == "") {
      if (users.empty() == false and users.back()->rank == 7) {
        new_password = current_password;
        current_password = "";
      } else {
        std::cout << "Invalid\n";
        return;
      }
    }
    if (valid_check(0, new_password)) {
      int pos = user_db.index_find(id);
      if (pos != -1) {
        if (current_password == "" or current_password == user_db.check(pos)) {
          int priority = user_db.get_priority(pos);
          user_db.remove(id);
          user_db.insert(id, "", new_password, priority);
          // suppose that the username will not be used
        } else {
          std::cout << "Invalid\n";
        }
      } else {
        std::cout << "Invalid\n";
      }
    } else {
      std::cout << "Invalid\n";
    }
  } else {
    std::cout << "Invalid\n";
    getline(std::cin, input);
  }
}

int main() {
  std::string input;

  while (true) {
    std::cin >> input;
    handleInput(input);
  }

  return 0;
}
