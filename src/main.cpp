#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "book_info.hpp"
#include "user.hpp"

std::vector<std::shared_ptr<user>> users;
Book book_db;
UserfileDatabase user_db("user_file.txt");

bool valid_check(int type, std::string &input) {
  if (input.size() > 30 and type < 3) {
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

  if (type == 3) {  // 3代表[ISBN]
    if (input == "") {
      return false;
    }
    if (input.size() > 20) {
      return false;
    }
    for (int i = 0; i < input.size(); i++) {
      if (input[i] < 32 or input[i] > 126) {
        return false;
      }
    }
    return true;
  }
  if (type == 4) {  // 4代表[BookName], [Author]
    if (input == "") {
      return false;
    }
    if (input.size() > 62) {
      return false;
    }
    if (input[0] != '\"' or input[input.size() - 1] != '\"') {
      return false;
    }
    for (int i = 1; i < input.size() - 1; i++) {
      if (input[i] < 32 or input[i] > 126 or input[i] == '\"') {
        return false;
      }
    }
    return true;
  }
  if (type == 5) {  // 5代表[Keyword]
    if (input == "") {
      return false;
    }
    if (input.size() > 62) {
      return false;
    }
    if (input[0] != '\"' or input[input.size() - 1] != '\"') {
      return false;
    }
    for (int i = 1; i < input.size() - 1; i++) {
      if (input[i] < 32 or input[i] > 126 or input[i] == '\"') {
        return false;
      }
      if (input[i] == '|' and (i == input.size() - 1 or input[i + 1] == '|')) {
        return false;
      }
    }
    return true;
  }
  if (type == 6) {  // 6代表[Quantity]
    if (input == "") {
      return false;
    }
    if (input.size() > 10) {
      return false;
    }
    if (input.size() == 10) {
      if (input > "2147483647") {
        return false;
      }
    }
    for (int i = 0; i < input.size(); i++) {
      if (input[i] < '0' or input[i] > '9') {
        return false;
      }
    }
    return true;
  }
  if (type == 7) {  // 7代表[Price], [TotalCost]
    if (input == "") {
      return false;
    }
    if (input.size() > 13) {
      return false;
    }
    int dot = -1;
    for (int i = 0; i < input.size(); i++) {
      if ((input[i] < '0' or input[i] > '9') and input[i] != '.') {
        return false;
      }
      if (input[i] == '.') {
        if (dot != -1) {
          return false;
        }
        dot = i;
        if (dot < input.size() - 3) {
          return false;
        }
      }
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
    if (users.empty() or users.back()->rank < 1) {
      std::cout << "Invalid\n";
      return;
    }
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
  } else if (input == "show") {
    getline(std::cin, input);
    if (users.empty() or users.back()->rank < 1) {
      std::cout << "Invalid\n";
      return;
    }
    std::istringstream stream(input);
    std::string info;
    stream >> info;
    if (input == "") {
      book_db.find(info, 4);
    } else {
      int split = -1;
      for (int i = 1; i < info.size(); i++) {
        if (info[i] == '=') {
          split = i;
          break;
        }
      }
      if (split == -1 or info[0] != '-') {
        std::cout << "Invalid\n";
        return;
      }
      std::string type = info.substr(1, split - 1);
      std::string index = info.substr(split + 1);
      if (index == "") {
        std::cout << "Invalid\n";
        return;
      }
      if (type == "ISBN") {
        if (valid_check(3, index) == false) {
          std::cout << "Invalid\n";
          return;
        }
        book_db.find(index, 0);
      } else if (type == "name") {
        if (valid_check(4, index) == false) {
          std::cout << "Invalid\n";
          return;
        }
        book_db.find(index, 1);
      } else if (type == "author") {
        if (valid_check(4, index) == false) {
          std::cout << "Invalid\n";
          return;
        }
        book_db.find(index, 2);
      } else if (type == "keyword") {
        if (valid_check(5, index) == false) {
          std::cout << "Invalid\n";
          return;
        }
        book_db.find(index, 3);
      } else {
        std::cout << "Invalid\n";
      }
    }
  } else if (input == "buy") {
    getline(std::cin, input);
    if (users.empty() or users.back()->rank < 1) {
      std::cout << "Invalid\n";
      return;
    }
    std::istringstream stream(input);
    std::string ISBN, quantity;
    stream >> ISBN >> quantity;
    int quantity_int = -1;
    if (valid_check(3, ISBN) == false) {
      std::cout << "Invalid\n";
      return;
    }
    if (valid_check(6, quantity) == false) {
      std::cout << "Invalid\n";
      return;
    }
    try {
      quantity_int = std::stoi(quantity);
    } catch (std::invalid_argument) {
      std::cout << "Invalid\n";
      return;
    }
    if (quantity_int <= 0) {
      std::cout << "Invalid\n";
      return;
    }
    book_db.buy(ISBN, quantity_int);
  } else if (input == "select") {
    getline(std::cin, input);
    if (users.empty() or users.back()->rank < 3) {
      std::cout << "Invalid\n";
      return;
    }
    std::istringstream stream(input);
    std::string ISBN;
    stream >> ISBN;
    if (valid_check(3, ISBN) == false) {
      std::cout << "Invalid\n";
      return;
    }
    if (ISBN == "") {
      std::cout << "Invalid\n";
      return;
    }
    if (book_db.return_index(ISBN) == -1) {
      book_db.new_book(ISBN);
    }
    users.back()->select = book_db.return_index(ISBN);
  } else if (input == "modify") {
    getline(std::cin, input);
    if (users.empty() or users.back()->rank < 3 or users.back()->select == -1) {
      std::cout << "Invalid\n";
      return;
    }
    std::istringstream stream(input);
    std::string info[5];
    std::string ISBN, name, author, keyword, price;
    stream >> info[0] >> info[1] >> info[2] >> info[3] >> info[4];
    if (info[0] == "") {
      std::cout << "Invalid\n";
      return;
    }
    for (int i = 0; i < 5; i++) {
      if (info[i] == "") {
        break;
      }
      int split = -1;
      for (int j = 1; j < info[i].size(); j++) {
        if (info[i][j] == '=') {
          split = j;
          break;
        }
      }
      if (split == -1 or info[i][0] != '-') {
        std::cout << "Invalid\n";
        return;
      }
      std::string type = info[i].substr(1, split - 1);
      std::string index = info[i].substr(split + 1);
      if (index == "") {
        std::cout << "Invalid\n";
        return;
      }
      if (type == "ISBN" and ISBN == "") {
        ISBN = index;
        if (valid_check(3, ISBN) == false) {
          std::cout << "Invalid\n";
          return;
        }
      } else if (type == "name" and name == "") {
        name = index;
        if (valid_check(4, name) == false) {
          std::cout << "Invalid\n";
          return;
        }
      } else if (type == "author" and author == "") {
        author = index;
        if (valid_check(4, author) == false) {
          std::cout << "Invalid\n";
          return;
        }
      } else if (type == "keyword" and keyword == "") {
        keyword = index;
        if (valid_check(5, keyword) == false) {
          std::cout << "Invalid\n";
          return;
        }
      } else if (type == "price" and price == "") {
        price = index;
        if (valid_check(7, price) == false) {
          std::cout << "Invalid\n";
          return;
        }
      } else {
        std::cout << "Invalid\n";
        return;
      }
    }
    book_db.modify(users.back()->select, ISBN, name, author, keyword, price);
  } else if (input == "import") {
    getline(std::cin, input);
    if (users.empty() or users.back()->rank < 3 or users.back()->select == -1) {
      std::cout << "Invalid\n";
      return;
    }
    std::istringstream stream(input);
    std::string quantity, totalcost;
    stream >> quantity >> totalcost;
    if (quantity == "" or totalcost == "") {
      std::cout << "Invalid\n";
      return;
    }
    if (valid_check(6, quantity) == false or
        valid_check(7, totalcost) == false) {
      std::cout << "Invalid\n";
      return;
    }
    int quantity_int = -1, totalcost_double = -1;
    try {
      quantity_int = std::stoi(quantity);
      totalcost_double = std::stod(totalcost);
    } catch (std::invalid_argument) {
      std::cout << "Invalid\n";
      return;
    }
    if (quantity_int <= 0 or totalcost_double <= 0) {
      std::cout << "Invalid\n";
      return;
    }
    book_db.import(users.back()->select, quantity_int);
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
