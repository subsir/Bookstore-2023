#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "user.cpp"

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
    if (input[0] < '0' or input[0] > '9') {
      return false;
    }
    return true;
    // 可能存在的robust错误：只能输入1、3、7
  }
  return false;
}

void handleInput(std::string &input) {
  if (input == "exit" or input == "quit") {
    exit(0);
  } else if (input == "su") {
    getline(std::cin, input);
    std::istringstream stream(input);
    std::string id, password;
    stream >> id >> password;
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
    std::string id, password, username;
    stream >> id >> password >> username;
    if (valid_check(0, id) and valid_check(0, password) and
        valid_check(1, username)) {
      if (user_db.index_find(id) == -1) {
        user_db.insert(id, username, password);
      } else {
        std::cout << "Invalid\n";
      }
    } else {
      std::cout << "Invalid\n";
    }
  } else if (input == "delete") {
    getline(std::cin, input);
    std::istringstream stream(input);
    std::string id;
    stream >> id;
    if (std::find(users.begin(), users.end(), id) == users.end()) {
      int pos = user_db.index_find(id);
      if (pos != -1) {
        user_db.remove(id);
      } else {
        std::cout << "Invalid\n";
      }
    } else {
      std::cout << "Invalid\n";
    }
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
