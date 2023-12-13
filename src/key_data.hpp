#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class KeyValueDatabase {
 private:
  struct Index {
    static const short max_size = 100;
    char index[max_size][61];
    int block[max_size];
    short size = 0;
    Index() : block{}, index{} {}
  };

  struct Entry {
    static const short max_size = 100;
    short top = 0;
    int value[max_size];
    int next = -1;
    Entry() : value{} {}
  };

  std::string filename;
  std::fstream file;
  std::vector<int> index_vec;
  std::unordered_map<std::string, int> index_map;

 public:
  std::vector<int> value_vec;
  KeyValueDatabase(const std::string &filename) : filename(filename) {
    if (!fileExists()) {
      file.open(filename, std::ios::out);
      file.close();
    } else {
      file.open(filename + "_index.txt", std::ios::in | std::ios::out);
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

  ~KeyValueDatabase() {
    file.close();
    file.open(filename + "_index.txt", std::ios::out);
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

  void insert(const std::string &index, const int &value) {
    index_find(index);
    bool add_entry = false;
    bool link_pre = false;
    Entry temp_entry;
    if (index_vec.empty()) {
      add_entry = true;
    } else {
      file.seekg(index_vec.back(), std::ios::beg);
      file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
      if (temp_entry.top == Entry::max_size) {
        link_pre = true;
      }
    }
    if (add_entry) {
      Entry temp_entry2;
      temp_entry2.value[temp_entry2.top++] = value;
      file.seekp(0, std::ios::end);
      index_map[index] = file.tellp();
      file.write(reinterpret_cast<char *>(&temp_entry2), sizeof(Entry));
    } else if (link_pre) {
      file.seekp(0, std::ios::end);
      temp_entry.next = file.tellp();
      Entry temp_entry2;
      temp_entry2.value[temp_entry2.top++] = value;
      file.write(reinterpret_cast<char *>(&temp_entry2), sizeof(Entry));
      file.seekp(index_vec.back(), std::ios::beg);
      file.write(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
    } else {
      temp_entry.value[temp_entry.top++] = value;
      file.seekp(index_vec.back(), std::ios::beg);
      file.write(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
    }
  }

  void remove(const std::string &index, const int &value) {
    index_find(index);
    for (short i = 0; i < index_vec.size(); ++i) {
      file.seekg(index_vec[i], std::ios::beg);
      Entry temp_entry;
      file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
      for (short it = 0; it < temp_entry.top; ++it) {
        if (temp_entry.value[it] == value) {
          if (temp_entry.top > 1) {
            temp_entry.value[it] = temp_entry.value[temp_entry.top - 1];
          }
          --temp_entry.top;
          file.seekp(index_vec[i], std::ios::beg);
          file.write(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
          return;
        }
      }
    }
  }

  void find(const std::string &index) {
    value_vec.clear();
    auto it = index_map.find(index);
    int shift_pos;
    if (it != index_map.end()) {
      Entry temp_entry;
      file.seekg(it->second, std::ios::beg);
      file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
      for (short it = 0; it < temp_entry.top; ++it) {
        value_vec.push_back(temp_entry.value[it]);
      }
      shift_pos = temp_entry.next;
      while (shift_pos != -1) {
        file.seekg(shift_pos, std::ios::beg);
        file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
        for (short it = 0; it < temp_entry.top; ++it) {
          value_vec.push_back(temp_entry.value[it]);
        }
        shift_pos = temp_entry.next;
      }
    }
  }
  void all() {
    value_vec.clear();
    for (auto it : index_map) {
      int shift_pos;
      Entry temp_entry;
      file.seekg(it.second, std::ios::beg);
      file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
      for (short it = 0; it < temp_entry.top; ++it) {
        value_vec.push_back(temp_entry.value[it]);
      }
      shift_pos = temp_entry.next;
      while (shift_pos != -1) {
        file.seekg(shift_pos, std::ios::beg);
        file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
        for (short it = 0; it < temp_entry.top; ++it) {
          value_vec.push_back(temp_entry.value[it]);
        }
        shift_pos = temp_entry.next;
      }
    }
  }

 private:
  inline bool fileExists() {
    std::ifstream file2(filename);
    return file2.good();  // 文件存在时返回 true，否则返回 false
  }
  void index_find(const std::string &index) {
    short i;
    int shift_pos = 0, pre_pos = 0;
    index_vec.clear();
    auto it = index_map.find(index);
    if (it != index_map.end()) {
      Entry temp_entry;
      index_vec.push_back(it->second);
      file.seekg(index_vec[0], std::ios::beg);
      file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
      shift_pos = temp_entry.next;
      while (shift_pos != -1) {
        file.seekg(shift_pos, std::ios::beg);
        file.read(reinterpret_cast<char *>(&temp_entry), sizeof(Entry));
        index_vec.push_back(shift_pos);
        shift_pos = temp_entry.next;
      }
    }
    return;
  }
};

class KeyValueDatabase2 {
 private:
  struct Index {
    static const short max_size = 100;
    char index[max_size][21];
    int block[max_size];
    short size = 0;
    Index() : block{}, index{} {}
  };

  std::string filename;
  std::fstream file;
  std::vector<int> index_vec;
  std::unordered_map<std::string, int> index_map;

 public:
  std::vector<int> value_vec;
  KeyValueDatabase2(const std::string &filename) : filename(filename) {
    if (!fileExists()) {
      file.open(filename, std::ios::out);
      file.close();
    } else {
      file.open(filename + "_index.txt", std::ios::in | std::ios::out);
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

  ~KeyValueDatabase2() {
    file.close();
    file.open(filename + "_index.txt", std::ios::out);
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

  void insert(const std::string &index, const int &value) {
    index_map[index] = value;
  }

  void remove(const std::string &index, const int &value) {
    index_map.erase(index);
  }

  void find(const std::string &index) {
    value_vec.clear();
    auto it = index_map.find(index);
    int shift_pos;
    if (it != index_map.end()) {
      value_vec.push_back(it->second);
    }
  }
  void all() {
    value_vec.clear();
    for (auto it : index_map) {
      value_vec.push_back(it.second);
    }
  }

 private:
  inline bool fileExists() {
    std::ifstream file2(filename);
    return file2.good();  // 文件存在时返回 true，否则返回 false
  }
};
