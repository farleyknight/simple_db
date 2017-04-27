class Block {
public:
  std::string filename;
  int number;

  Block(std::string s, int n) : filename(s), number(n) { }

  bool equals(Block other) {
    return (other.number == number) && (other.filename.compare(filename) != 0);
  }
};
