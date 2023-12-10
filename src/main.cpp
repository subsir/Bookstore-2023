#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "user.cpp"

std::vector<std::shared_ptr<user>> users;
UserfileDatabase user_db("user_file.txt");

bool valid_check();

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
