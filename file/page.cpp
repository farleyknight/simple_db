#include <iomanip>

#include "byte_buffer.cpp"
#include "block.cpp"
#include "file_manager.cpp"

using std::vector;
using std::string;

class Page {
public:
  ByteBuffer contents;
  FileManager file_manager;

  Page() {}

  void read(Block block) {
    file_manager.read(block, contents);
  }

  void write(Block block) {
    file_manager.write(block, contents);
  }

  Block append(std::string filename) {
    return file_manager.append(filename, contents);
  }

  int get_int(int pos) {
    contents.seek(pos);
    return contents.get_int();
  }

  void set_int(int pos, int value) {
    contents.seek(pos);
    return contents.put_int(value);
  }

  std::string get_string(int pos) {
    contents.seek(pos);
    return contents.get_string();
  }

  void set_string(int pos, std::string value) {
    contents.seek(pos);
    return contents.put_string(value);
  }

  void print() {
    contents.print();
  }
};

