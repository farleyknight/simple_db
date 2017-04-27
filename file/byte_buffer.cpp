#include <string>
#include <array>

using byte = unsigned char;

class ByteBuffer {
public:
  const static int BLOCK_SIZE = 1000;
  const static int INT_SIZE   = sizeof(int);
  static int STRING_SIZE(int n) {
    return INT_SIZE + (n * sizeof(char));
  }
  
  std::array<byte, BLOCK_SIZE> contents;
  int position = 0;
    
  ByteBuffer() {
    clear();
  }

  void clear() {
    contents.fill(0);
  }

  void seek(int i) {
    position = i;
  }

  void rewind() {
    position = 0;
  }

  int capacity() {
    return BLOCK_SIZE;
  }

  byte &operator[](int i) {
    return contents[i];
  }

  // TODO: Memorize this!
  void put_int(int value) {
    contents[position + 3] = value;
    contents[position + 2] = (value >> 8);
    contents[position + 1] = (value >> 16);
    contents[position + 0] = (value >> 24);
  }

  // TODO: Memorize this!
  int get_int() {
    int value =
      static_cast<int>(static_cast<byte>(contents[position+0]) << 24 |
		       static_cast<byte>(contents[position+1]) << 16 |
		       static_cast<byte>(contents[position+2]) << 8 |
		       static_cast<byte>(contents[position+3]));      
    return value;
  }

  void put_string(std::string value) {
    put_int(value.size());      
    for (int i = 0; i < value.size(); i++) {
      contents[position + i + INT_SIZE] = value[i];
    }
  }

  std::string get_string() {
    int length = get_int();
    std::string result;
    for (int i = 0; i < length; i++) {
      result += contents[position + i + INT_SIZE];
    }
    return result;
  }

  void print() {
    std::cout << std::hex << std::setfill('0');
  
    for (int i = 0; i < ByteBuffer::BLOCK_SIZE; i++) {
      if (i != 0)
	std::cout << ", ";
      
      std::cout << "0x" << std::setw(2)
		<< std::hex << static_cast<unsigned>(contents[i]);
    }
  }
};
