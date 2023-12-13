#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

#include "book_info_storage.hpp"
#include "key_data.hpp"

class Book {
 private:
  struct book {
    char ISBN[20];
    char name[60];
    char author[60];
    char keyword[60];
    long double price = 0;
    int quantity = 0;
    bool operator<(const book &rhs) const { return strcmp(ISBN, rhs.ISBN) < 0; }
    book() {
      ISBN[0] = '\0';
      name[0] = '\0';
      author[0] = '\0';
      keyword[0] = '\0';
    }
  };

  struct log {
    long double profit = 0;
    long double expense = 0;
  };

  std::vector<std::string> keyword_vec;
  std::vector<book> book_vec;
  KeyValueDatabase ISBN_db;
  KeyValueDatabase name_db;
  KeyValueDatabase author_db;
  KeyValueDatabase keyword_db;
  MemoryRiver<book> book_db;
  MemoryRiver<log> log_db;

 public:
  Book()
      : ISBN_db("ISBN_db"),
        name_db("name_db"),
        author_db("author_db"),
        keyword_db("keyword_db"),
        book_db("book_db"),
        log_db("log_db") {}

  void new_book(std::string &index) {
    book temp;
    strcpy(temp.ISBN, index.c_str());
    int pos = book_db.write(temp);
    ISBN_db.insert(index, pos);
  }

  void find(std::string &index, int type) {
    book_vec.clear();
    if (type == 0) {
      ISBN_db.find(index);
      for (auto i : ISBN_db.value_vec) {
        book temp;
        book_db.read(temp, i);
        book_vec.push_back(temp);
      }
    } else if (type == 1) {
      name_db.find(index);
      for (auto i : name_db.value_vec) {
        book temp;
        book_db.read(temp, i);
        book_vec.push_back(temp);
      }
    } else if (type == 2) {
      author_db.find(index);
      for (auto i : author_db.value_vec) {
        book temp;
        book_db.read(temp, i);
        book_vec.push_back(temp);
      }
    } else if (type == 3) {
      keyword_db.find(index);
      for (auto i : keyword_db.value_vec) {
        book temp;
        book_db.read(temp, i);
        book_vec.push_back(temp);
      }
    } else {
      ISBN_db.all();
      for (auto i : ISBN_db.value_vec) {
        book temp;
        book_db.read(temp, i);
        book_vec.push_back(temp);
      }
    }
    std::sort(book_vec.begin(), book_vec.end());
    for (auto i : book_vec) {
      std::cout << i.ISBN << '\t' << i.name << '\t' << i.author << '\t'
                << i.keyword << '\t';
      printf("%.2Lf\t%d\n", i.price, i.quantity);
    }
    if (book_vec.empty()) {
      std::cout << '\n';
    }
  }

  int return_index(std::string &index) {
    ISBN_db.find(index);
    if (ISBN_db.value_vec.empty()) {
      return -1;
    }
    return ISBN_db.value_vec[0];
  }
  void buy(std::string &index, int quantity) {
    ISBN_db.find(index);
    if (ISBN_db.value_vec.empty()) {
      std::cout << "Invalid\n";
      return;
    }
    book temp;
    book_db.read(temp, ISBN_db.value_vec[0]);
    if (temp.quantity < quantity) {
      std::cout << "Invalid\n";
      return;
    }
    temp.quantity -= quantity;
    book_db.revise(temp, ISBN_db.value_vec[0]);
    long double price = temp.price * quantity;
    printf("%.2Lf\n", price);
    int num, pos;
    log temp_log;
    log_db.get_info(num, 1);
    log_db.get_info(pos, 2);
    if (num > 0) {
      log_db.read(temp_log, pos);
    }
    temp_log.profit += price;
    pos = log_db.write(temp_log);
    num++;
    log_db.write_info(num, 1);
    log_db.write_info(pos, 2);
  }

  void import(int num, int quantity, long double price) {
    book temp;
    book_db.read(temp, num);
    temp.quantity += quantity;
    book_db.revise(temp, num);
    int num2, pos;
    log temp_log;
    log_db.get_info(num2, 1);
    log_db.get_info(pos, 2);
    if (num2 > 0) {
      log_db.read(temp_log, pos);
    }
    temp_log.expense += price;
    pos = log_db.write(temp_log);
    num2++;
    log_db.write_info(num2, 1);
    log_db.write_info(pos, 2);
  }

  void modify(int num, std::string &ISBN, std::string &name,
              std::string &author, std::string &keyword, std::string &price) {
    book temp;
    book_db.read(temp, num);
    if (ISBN != "") {
      if (temp.ISBN == ISBN) {
        std::cout << "Invalid\n";
        return;
      }
      ISBN_db.find(ISBN);
      if (ISBN_db.value_vec.empty() == false) {
        std::cout << "Invalid\n";
        return;
      }
      ISBN_db.remove(temp.ISBN, num);
      strcpy(temp.ISBN, ISBN.c_str());
      ISBN_db.insert(ISBN, num);
    }
    if (name != "") {
      name_db.remove(temp.name, num);
      std::string index = name.substr(1, name.length() - 2);
      strcpy(temp.name, index.c_str());
      name_db.insert(name, num);
    }
    if (author != "") {
      author_db.remove(temp.author, num);
      std::string index = author.substr(1, author.length() - 2);
      strcpy(temp.author, index.c_str());
      author_db.insert(author, num);
    }
    if (keyword != "") {
      keyword_vec.clear();
      int split = -1;
      std::string old_keyword = temp.keyword;
      while (old_keyword != "") {
        split = old_keyword.find('|');
        if (split == -1) {
          if (std::find(keyword_vec.begin(), keyword_vec.end(), old_keyword) ==
              keyword_vec.end()) {
            keyword_vec.push_back(old_keyword);
          } else {
            std::cout << "Invalid\n";
            return;
          }
          old_keyword = "";
        } else {
          keyword_vec.push_back(old_keyword.substr(0, split));
          old_keyword = old_keyword.substr(split + 1);
        }
      }
      for (auto i : keyword_vec) {
        if (i[0] != '\"') {
          i = '\"' + i;
        }
        if (i[i.length() - 1] != '\"') {
          i = i + '\"';
        }
        keyword_db.remove(i, num);
      }
      std::string index = keyword.substr(1, keyword.length() - 2);
      strcpy(temp.keyword, index.c_str());
      keyword_vec.clear();
      split = -1;
      while (keyword != "") {
        split = keyword.find('|');
        if (split == -1) {
          if (std::find(keyword_vec.begin(), keyword_vec.end(), keyword) ==
              keyword_vec.end()) {
            keyword_vec.push_back(keyword);
          } else {
            std::cout << "Invalid\n";
            return;
          }
          keyword = "";
        } else {
          keyword_vec.push_back(keyword.substr(0, split));
          keyword = keyword.substr(split + 1);
        }
      }
      for (auto i : keyword_vec) {
        if (i[0] != '\"') {
          i = '\"' + i;
        }
        if (i[i.length() - 1] != '\"') {
          i = i + '\"';
        }
        keyword_db.insert(i, num);
      }
    }
    if (price != "") {
      long double temp_price = -1;
      try {
        temp_price = std::stold(price);
      } catch (std::invalid_argument) {
        std::cout << "Invalid\n";
        return;
      }
      temp.price = temp_price;
    }
    book_db.revise(temp, num);
  }

  void finance(int count) {
    int num, pos;
    log temp_log, temp_log2;
    log_db.get_info(num, 1);
    log_db.get_info(pos, 2);
    if (num < count) {
      std::cout << "Invalid\n";
      return;
    }
    log_db.read(temp_log, pos);
    if (count == -1 or count == num) {
      printf("+ %.2Lf ", temp_log.profit);
      printf("- %.2Lf\n", temp_log.expense);
      return;
    }
    log_db.read(temp_log2, pos - count);
    long double profit = temp_log.profit - temp_log2.profit;
    long double expense = temp_log.expense - temp_log2.expense;
    printf("+ %.2Lf ", profit);
    printf("- %.2Lf\n", expense);
    return;
  }
};