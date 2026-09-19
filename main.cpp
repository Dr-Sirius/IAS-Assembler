#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <print>
#include <vector>

using std::ifstream;

enum class TOKEN_TYPE { OPCODE, ARG, MARG, LOC, VAR, NONE };

struct Token {
  TOKEN_TYPE token;
  std::string val;
};

enum class BASE_OPCODE : int8_t {
  HALT = 0,
  LOAD = 1,
  ADD = 2,
  SUB = 3,
  MUL = 4,
  DIV = 5,
  JUMP = 6,
  STOR = 7,
  LSH = 8,
  RSH = 9,
  NONE = 10
};

enum class OPCODES : int8_t {
  HALT = 0b0, // 0  - HALT
  // LOAD AC INSTRS
  LOAD_M = 0b00000001,     // 1  - load M into AC
  LOAD_NEGM = 0b00000010,  // 2  - load -M into AC
  LOAD_ABSM = 0b00000011,  // 3  - load |M| into AC
  LOAD_NABSM = 0b00000100, // 4  - load -|M| into AC

  // ADD/SUB INSTRS
  ADD_M = 0b00000101,    // 5  - Add M to AC, load result into AC
  SUB_M = 0b00000110,    // 6  - Sub M from AC, load result into AC
  ADD_MABS = 0b00000111, // 7  - Add |M| to AC, load result into AC
  SUB_MABS = 0b00001000, // 8  - Sub |M| from AC, load result into AC

  // LOAD MQ INSTRS
  LOAD_MQM = 0b00001001, // 9  - load M into MQ
  LOAD_MQ = 0b00001010,  // 10 - load MQ into AC

  // Mul/Div INSTRS
  MUL_M = 0b00001011, // 11 - Multiply MQ by M, Load upper 32 bits into AC,
                      // lower into MQ
  DIV_M = 0b00001100, // 12 - Divide MQ by M, load quotient into MQ, Remainder
                      // into AC

  // JUMP INSTRS
  JUMP_ML = 0b00001101,  // 13 - Unconditional jump to M left instr
  JUMP_MR = 0b00001110,  // 14 - Unconditional jump to M right instr
  JUMP_PML = 0b00001111, // 15 - If AC >= 0 jump to M left instr
  JUMP_PMR = 0b00010000, // 16 - If AC >= 0 jump to M right instr

  // Address Modify INSTRS
  STOR_ML = 0b00010010, // 18 - Replace M at left instr with AC
  STOR_MR = 0b00010011, // 19 - Replace M at right instr with AC

  LSH = 0b00010100, // 20 - Left Hand Shift, Multiply AC by 2
  RSH = 0b00010101, // 21 - Right Hand Shift, Divide AC by 2

  // Stor INSTR
  STOR_M = 0b00100001, // 33 - Store AC at M
};

struct INSTRUCTION {
  OPCODES opcode;
  int8_t operand = 0b0;
};

BASE_OPCODE strToBASEOPCODE(std::string str) {
  if (str == "HALT") {
    return BASE_OPCODE::HALT;
  }
  if (str == "LOAD") {
    return BASE_OPCODE::LOAD;
  }
  if (str == "ADD") {
    return BASE_OPCODE::ADD;
  }
  if (str == "SUB") {
    return BASE_OPCODE::SUB;
  }
  if (str == "MUL") {
    return BASE_OPCODE::MUL;
  }
  if (str == "DIV") {
    return BASE_OPCODE::DIV;
  }
  if (str == "JUMP") {
    return BASE_OPCODE::JUMP;
  }
  if (str == "STOR") {
    return BASE_OPCODE::STOR;
  }
  if (str == "LSH") {
    return BASE_OPCODE::LSH;
  }
  if (str == "RSH") {
    return BASE_OPCODE::RSH;
  }
  return BASE_OPCODE::NONE;
}

std::string removeComment(std::string line) {
  std::string nString;
  for (const char &c : line) {
    if (c == ';')
      break;
    nString += c;
  }
  return nString;
}

std::vector<std::string> splitLine(const std::string &line) {

  std::stringstream stream(line);

  std::string temp;

  std::vector<std::string> split;

  while (stream >> temp)
    split.push_back(temp);

  return split;
}

std::pair<Token, Token> getMemArg(const std::string &str) {

  std::string arg;
  std::string marg;

  for (const char &c : str) {
    if (isdigit(c)) {
      marg += c;
    } else {
      arg += c;
    }
  }
  return {{TOKEN_TYPE::ARG, arg}, {TOKEN_TYPE::MARG, marg}};
}

void tokenize(std::vector<Token> &tokens, const std::vector<std::string> &inp) {
  tokens.push_back({TOKEN_TYPE::LOC, inp[0]});
  if (inp.size() == 2) {
    std::println("INP {}", inp);
    tokens.push_back({TOKEN_TYPE::VAR, inp[1]});
    return;
  }

  for (int i = 1; i < inp.size(); ++i) {
    BASE_OPCODE code = strToBASEOPCODE(inp[i]);
    if (code == BASE_OPCODE::NONE) {
      std::pair<Token, Token> args = getMemArg(inp[i]);
      tokens.push_back(args.first);
      tokens.push_back(args.second);
    } else {
      tokens.push_back({TOKEN_TYPE::OPCODE, inp[i]});
    }
  }
}

void convertTokensToBytes(const std::vector<Token> &tokens) {
  std::vector<Token> memTokens;
  std::vector<Token> instrs;

  for (int i = 0; i < tokens.size(); ++i) {

    if (tokens[i].token == TOKEN_TYPE::LOC &&
        tokens[i + 1].token == TOKEN_TYPE::VAR) {
      memTokens.push_back(tokens[i]);
      memTokens.push_back(tokens[i + 1]);
      ++i;
      continue;
    }
    instrs.push_back(tokens[i]);
  }
  std::print("[");
  for (const Token &t : memTokens) {
    std::print("[{},{}]", (int)t.token, t.val);
  }
  std::println("]");

  std::print("[");
  for (const Token &t : instrs) {
    std::print("[{},{}]", (int)t.token, t.val);
  }
  std::println("]");
}

int main() {
  ifstream file("example.txt");
  std::string st;
  std::vector<Token> tokens;

  while (getline(file, st)) {

    std::string nString = removeComment(st);
    if (nString.length() > 0) {
      std::println("{}", nString);
      std::println("split {}", splitLine(nString));
      tokenize(tokens, splitLine(nString));
    }
  }
  // std::print("[");
  // for (const Token &t : tokens) {
  //   std::print("[{},{}]", (int)t.token, t.val);
  // }
  // std::println("]");
  convertTokensToBytes(tokens);
}
