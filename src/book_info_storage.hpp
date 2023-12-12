#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <ios>
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;

template <class T, int info_len = 2>
class MemoryRiver {
 private:
  int add_len = 0;
  fstream file;
  string file_name;
  int sizeofT = sizeof(T);

 public:
  MemoryRiver(const string &file_name) : file_name(file_name) {
    // if (!fileExists()) {
    initialise();
    // }
    file.open(file_name, std::ios::in | std::ios::out);
  }
  ~MemoryRiver() { file.close(); }

  void initialise(string FN = "") {
    if (FN != "") file_name = FN;
    file.open(file_name, std::ios::out);
    int tmp = 0;
    for (int i = 0; i < info_len; ++i)
      file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    file.close();
  }

  // 读出第n个int的值赋给tmp，1_base
  void get_info(int &tmp, int n) {
    if (n > info_len + add_len) return;
    file.seekg((n - 1) * sizeof(int));
    file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
  }

  // 将tmp写入第n个int的位置，1_base
  void write_info(int tmp, int n) {
    if (n > info_len + add_len) return;
    file.seekg((n - 1) * sizeof(int));
    file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
  }

  // 在文件合适位置写入类对象t，并返回写入的位置索引index
  // 位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
  // 位置索引index可以取为对象写入的起始位置
  int write(T &t) {
    file.seekp(0, std::ios::end);
    int index = ((int)file.tellp() - info_len * sizeof(int)) / sizeofT;
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
    add_len += sizeofT / sizeof(int);
    return index;
  }

  void revise(T &t, const int index) {
    file.seekp(index * sizeofT + info_len * sizeof(int));
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
  }

  // 用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
  void update(T &t, const int index) {
    file.seekp(index * sizeofT + info_len * sizeof(int));
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
  }

  // 读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
  void read(T &t, const int index) {
    file.seekg(index * sizeofT + info_len * sizeof(int));
    file.read(reinterpret_cast<char *>(&t), sizeof(T));
  }

 private:
  inline bool fileExists() {
    std::ifstream file2(file_name);
    return file2.good();  // 文件存在时返回 true，否则返回 false
  }
};

#endif  // BPT_MEMORYRIVER_HPP