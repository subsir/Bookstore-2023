#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class UserfileDatabase {
 private:
  struct Index {
    static const short max_size = 100;
    char index[max_size][61];
    int block[max_size];
    short size = 0;
    Index() : block{}, index{} {}
  };

  struct Entry {
    int priority;
    char userid[31];
    char username[31];
    char password[31];
    Entry() : priority(0), userid{}, username{}, password{} {}
  };

  std::string filename;
  std::fstream file;
  std::unordered_map<std::string, int> index_map;

 public:
  UserfileDatabase(const std::string &filename) : filename(filename) {
    if (!fileExists()) {
      file.open(filename, std::ios::out);
      file.close();
      file.open("_userfile_index", std::ios::out);
      file.close();
      file.open(filename, std::ios::in | std::ios::out);
      insert("root", "admin", "sjtu", 7);
      file.close();
    } else {
      file.open("_userfile_index", std::ios::in | std::ios::out);
      int num;
      file.read(reinterpret_cast<char *>(&num), sizeof(int));
      Index temp_index;
      for (int i = 0; i < num; i += Index::max_size) {
        file.read(reinterpret_cast<char *>(&temp_index), sizeof(Index));
        for (int j = 0; j < Index::max_size and i + j < num; j++) {
          index_map[temp_index.index[j]] = temp_index.block[j];
        }
      }
      file.close();
    }
    file.open(filename, std::ios::in | std::ios::out);
  }

  ~UserfileDatabase() {
    file.close();
    file.open("_userfile_index", std::ios::out);
    int num = index_map.size();
    file.write(reinterpret_cast<char *>(&num), sizeof(int));
    auto it = index_map.begin();
    for (int i = 0; i < num; i += Index::max_size) {
      Index temp_index;
      for (int j = 0; j < Index::max_size and i + j < num; j++) {
        strcpy(temp_index.index[j], it->first.c_str());
        temp_index.block[j] = it->second;
        it++;
      }
      file.write(reinterpret_cast<char *>(&temp_index), sizeof(Index));
    }
    file.close();
  }

  void insert(const std::string &index, const std::string &usname,
              const std::string &psword, int priority) {
    Entry temp_entry;
    index.copy(temp_entry.userid, index.length());
    usname.copy(temp_entry.username, usname.length());
    psword.copy(temp_entry.password, psword.length());
    temp_entry.priority = priority;
    file.seekp(0, std::ios::end);
    index_map[index] = file.tellp();
    file.write(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
  }

  void remove(const std::string &index) { index_map.erase(index); }

  std::string check(int pos) {
    file.seekg(pos);
    Entry temp_entry;
    file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
    return temp_entry.password;
  }

  int get_priority(int pos) {
    file.seekg(pos);
    Entry temp_entry;
    file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
    return temp_entry.priority;
  }
  int index_find(const std::string &index) {
    auto it = index_map.find(index);
    if (it != index_map.end()) {
      return it->second;
    }
    return -1;
  }

 private:
  inline bool fileExists() {
    std::ifstream file2(filename);
    return file2.good();  // 文件存在时返回 true，否则返回 false
  }
};
