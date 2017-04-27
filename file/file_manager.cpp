#include <iostream>
#include <map>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <vector>
#include <dirent.h>
#include <array>

using std::string;
using std::vector;

class FileManager {
public:
  class File {
  public:
    std::string name;
    std::fstream handle;

    void open(std::string filename) {
      name = filename;
      handle.open(name, std::ios::binary | std::ios::out | std::ios::in);
    }

    bool is_open() {
      return handle.is_open();
    }

    int length() {
      handle.seekg(0, handle.end);
      int length = handle.tellg();
      handle.seekg(0, handle.beg);
      return length;
    }

    void read(ByteBuffer buffer, int size) {
      handle.read(reinterpret_cast<char *>(&buffer.contents), size);
    }

    void write(ByteBuffer buffer, int size) {
      handle.write(reinterpret_cast<char *>(&buffer.contents), size);
    }
  };
  
  class FileTable {
  public:
    const static int TABLE_SIZE = 101;
    const static int SEED = 501;
    
    std::array<File, TABLE_SIZE> table;

    unsigned int hash(std::string filename) {
      unsigned int result = 0;
      for (int i = 0; i < filename.size(); i++) {
	result = (result * SEED) + int(filename[i]);
      }
      return result % TABLE_SIZE;
    }

    File &get(std::string filename) {
      int index = hash(filename);
      if (!table[index].is_open()) {
	table[index].open(filename);
      }
      return table[index];
    }

    bool contains(std::string filename) {
      int index = hash(filename);
      return table[index].is_open();
    }
    
  };
  
  bool existing;
  std::string home_directory, db_directory_path;
  FileTable files;

  FileManager() {
    FileManager("my_database");
  }
  
  FileManager(std::string db_name) : home_directory(std::getenv("HOME")),
				     db_directory_path(home_directory + "/" + db_name) {
    // If we ever want this code to work on Windows, it damn well better
    // live in a utility class somewhere..
    std::fstream db_directory(db_directory_path);
    // Check if the directory exists
    if (!db_directory.good()) {
      // Create if it doesn't
      // TODO: Create a FileUtil class that has a mkdir method
      // and can return the error string for `mkdir`
      int status = mkdir(db_directory_path.c_str(), ACCESSPERMS);
      if (status) {
	if (errno == EEXIST) {
	  existing = true;
	} else {
	  std::cout << "Got errno: " << errno << std::endl;
	  std::cout << "Could not create directory " << db_directory_path << std::endl;
	  exit(1);
	}
      } else {
	existing = false;
      }
    } else {
      existing = true;
    }

    // In the DB directory, delete any files starting with 'temp'
    vector<std::string> filenames = files_in(db_directory_path);
    
    for (std::string filename : filenames) {
      if (filename.find("temp") == 0) {
	remove(filename.c_str());
      }
    }
  }

  // TODO: Extract this file system crap into it's own object
  vector<std::string> files_in(std::string path) {
    vector<std::string> result;

    DIR *directory;
    struct dirent *entry;

    directory = opendir(path.c_str());
    if (directory != NULL) {
      while ((entry = readdir(directory)) != NULL) {
	std::string filename(entry->d_name);
	result.push_back(filename);
      }
    }
    
    return result;
  }

  void read(Block block, ByteBuffer buffer) {
    buffer.clear();
    files.get(block.filename)
      .read(buffer, ByteBuffer::BLOCK_SIZE);
  }

  void write(Block block, ByteBuffer buffer) {
    buffer.rewind();
    files.get(block.filename)
      .write(buffer, ByteBuffer::BLOCK_SIZE);
  }

  Block append(std::string filename, ByteBuffer buffer) {
    int block_number = size(filename);
    Block block(filename, block_number);
    write(block, buffer);
    return block;
  }
  
  int size(std::string filename) {
    return (files.get(filename).length() / ByteBuffer::BLOCK_SIZE);
  }
};
