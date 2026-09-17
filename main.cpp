#include <fstream>
#include <iostream>
#include <print>

using std::ifstream;

std::string removeComment(std::string line) {
  std::string nString;
  for (const char &c : line) {
    if (c == ';')
      break;
    nString += c;
  }
  return nString;
}

int main() {
  ifstream file("example.txt");
  std::string st;
  while (getline(file, st)) {

    std::string nString = removeComment(st);
    if (nString.length() > 0)
      std::println("{}", nString);
  }
}
