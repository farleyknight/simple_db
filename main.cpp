#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <fstream>
#include <cerrno>

#include "file/page.cpp"
#include "log/log_manager.cpp"

void test1() {
  Page p;  

  p.set_int(10, 42);
  assert(p.get_int(10) == 42);
  
  p.set_string(25, "hello");
  assert(p.get_string(25) == "hello");
}

void test2() {
  Page p1;
  std::cout << "creating block with junk, 0" << std::endl;
  Block block("junk", 0);

  std::cout << "p1 reading block" << std::endl;
  p1.read(block);

  std::cout << "p1 getting int at 792" << std::endl;
  int n = p1.get_int(792);

  std::cout << "p1 setting int at 792" << std::endl;
  p1.set_int(792, n+1);

  std::cout << "p1 getting int at 792; n = " << p1.get_int(792) << std::endl;

  std::cout << "p1 writing block" << std::endl;  
  p1.write(block);
}

int main() {
  test1();
  test2();
}

