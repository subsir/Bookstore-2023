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
    double price;
    int quantity = 0;
    bool operator<(const book &rhs) const { return strcmp(ISBN, rhs.ISBN) < 0; }
  };

  std::vector<std::string> keyword_vec;
  std::vector<book> book_vec;
  KeyValueDatabase ISBN_db;
  KeyValueDatabase name_db;
  KeyValueDatabase author_db;
  KeyValueDatabase keyword_db;
  MemoryRiver<book> book_db;

 public:
  Book()
      : ISBN_db("ISBN_db"),
        name_db("name_db"),
        author_db("author_db"),
        keyword_db("keyword_db"),
        book_db("book_db") {}

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
      printf("%.2lf\t%d\n", i.price, i.quantity);
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
    printf("%.2lf\n", temp.price * quantity);
  }

  void import(int num, int quantity) {
    book temp;
    book_db.read(temp, num);
    temp.quantity += quantity;
    book_db.revise(temp, num);
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
      double temp_price = -1;
      try {
        temp_price = std::stod(price);
      } catch (std::invalid_argument) {
        std::cout << "Invalid\n";
        return;
      }
      temp.price = temp_price;
    }
    book_db.revise(temp, num);
  }
};