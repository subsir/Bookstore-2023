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
                << i.keyword << '\t' << i.price << i.quantity << '\n';
    }
    if (book_vec.empty()) {
      std::cout << '\n';
    }
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

  void import(std::string &index, int quantity) {
    ISBN_db.find(index);
    if (ISBN_db.value_vec.empty()) {
      std::cout << "Invalid\n";
      return;
    }
    book temp;
    book_db.read(temp, ISBN_db.value_vec[0]);
    temp.quantity += quantity;
    book_db.revise(temp, ISBN_db.value_vec[0]);
  }
};