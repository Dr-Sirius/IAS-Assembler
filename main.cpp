#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <print>
#include <vector>
#include <cstdint>
#include <stack>

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

enum OPCODES : int8_t {
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

  bool stopDigits = false;

  for (int c = 0; c < str.length(); ++c) {
    if (isdigit(str[c]) && !stopDigits) {
      if (str[c+1]==',') {
        stopDigits = true;
      }
      marg += str[c];
    } else {
      if (str[c] == ',') continue;
      arg += str[c];
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

OPCODES getLoadOPCODE(std::string val) {
  if (val == "M()") return OPCODES::LOAD_M;
  if (val == "-M()") return OPCODES::LOAD_NEGM;
  if (val == "|M()|") return OPCODES::LOAD_ABSM;
  if (val == "-|M()|") return OPCODES::LOAD_NABSM;

  if (val == "MQ") return OPCODES::LOAD_MQ;
  if (val == "MQM()") return OPCODES::LOAD_MQM;

  return OPCODES::HALT;
}

OPCODES getArithOPCODE(BASE_OPCODE opcode,std::string val) {
  if (opcode == BASE_OPCODE::ADD && val == "M()") return OPCODES::ADD_M;
  if (opcode == BASE_OPCODE::ADD && val == "|M()|") return OPCODES::ADD_MABS;
  
  if (opcode == BASE_OPCODE::SUB && val == "M()") return OPCODES::SUB_M;
  if (opcode == BASE_OPCODE::SUB && val == "|M()|") return OPCODES::SUB_MABS;

  if (opcode == BASE_OPCODE::MUL && val == "M()") return OPCODES::MUL_M;
  if (opcode == BASE_OPCODE::DIV && val == "M()") return OPCODES::DIV_M;

  return OPCODES::HALT;
}

OPCODES getJumpOPCODE(std::string val) {
  if (val == "M(0:19)") return OPCODES::JUMP_ML;
  if (val == "M(20:39)") return OPCODES::JUMP_MR;

  if (val == "+M(0:19)") return OPCODES::JUMP_PML;
  if (val == "+M(20:39)") return OPCODES::JUMP_PMR;

  return OPCODES::HALT;
}

OPCODES getStorOPCODE(std::string val) {
  if (val == "M()") return OPCODES::STOR_M;

  if (val == "M(0:19)") return OPCODES::STOR_ML;
  if (val == "M(20:39)") return OPCODES::STOR_MR;

  return OPCODES::HALT;
}

OPCODES convertBaseOPTOOP(Token opcode, Token arg) {
  BASE_OPCODE op = strToBASEOPCODE(opcode.val);
  switch (op) {
  case BASE_OPCODE::LOAD:
    return getLoadOPCODE(arg.val);
    break;
  
  case BASE_OPCODE::ADD:
    return getArithOPCODE(op,arg.val);
    break;
  
  case BASE_OPCODE::SUB:
   return getArithOPCODE(op,arg.val);
    break;
  
  case BASE_OPCODE::MUL:
    return getArithOPCODE(op,arg.val);
    break;
  
  case BASE_OPCODE::DIV:
    return getArithOPCODE(op,arg.val);
    break;
  
  case BASE_OPCODE::JUMP:
    return getJumpOPCODE(arg.val);
    break;

  case BASE_OPCODE::STOR:
    return getStorOPCODE(arg.val);
    break;

  case BASE_OPCODE::LSH:
    return OPCODES::LSH;
    break;

  case BASE_OPCODE::RSH:
    return OPCODES::RSH;
    break;

  default:
    return OPCODES::HALT;
    break;
  }
}

int64_t loadInstrIntoBytes(int32_t address, INSTRUCTION instr1 = {HALT},
                      INSTRUCTION instr2 = {HALT})
{
  int64_t mem = ((static_cast<int64_t>(address) << 32)) | (instr1.opcode << 24) | (instr1.operand << 16) | (instr2.opcode << 8) | instr2.operand;
  return mem;
}

std::string binaryString(int32_t n)
{
  // return std::to_string(n);
  n = abs(n);
  std::string symb = "012";
  std::stack<char> s;
  std::string bin = "";
  int space = 0;
  for (; n / 2 > 0; n /= 2) {
    if (space == 4) {
        s.push(' ');
        space = 0;
    }
    ++space;
    s.push(symb[n % 2]);
      
  }
  s.push(symb[n % 2]);
  for (; !s.empty(); s.pop())
    bin += s.top();
  return bin;
}

void convertTokensToInstructions(const std::vector<Token> &tokens) {
  std::vector<int64_t> instrs;

  int32_t addr;
  INSTRUCTION instr1;
  INSTRUCTION instr2;
  bool load = false;

  for (int i = 0; i< tokens.size(); ++i) {
    if (load) {
      instrs.push_back(loadInstrIntoBytes(addr,instr1,instr2));
      load = false;
    }
    if (tokens[i].token == TOKEN_TYPE::LOC) {
      addr = std::stoi(tokens[i].val);
    }
    if (tokens[i].token == TOKEN_TYPE::OPCODE && tokens[i-1].token == TOKEN_TYPE::LOC) {
      std::println("HERE1");
      OPCODES op = convertBaseOPTOOP(tokens[i],tokens[i+1]);
      
      if (i+2 < tokens.size() && tokens[i+2].token == TOKEN_TYPE::MARG) {
        std::println("VAL{}",std::stoi(tokens[i+2].val));
        int8_t memadd = std::stoi(tokens[i+2].val);
        instr1 = {op,memadd};
        ++i;
        continue;
      }
      instr1 = {op};
      continue;
    }
    if (i+2 < tokens.size() && tokens[i].token == TOKEN_TYPE::OPCODE && tokens[i-1].token != TOKEN_TYPE::LOC) {
      std::println("HERE1");
      OPCODES op = convertBaseOPTOOP(tokens[i],tokens[i+1]);
      std::println("TYPE {}",(int)tokens[i+2].token);
      if (tokens[i+1].token == TOKEN_TYPE::MARG) {
        std::println("VAL{}",std::stoi(tokens[i+2].val));
        int8_t memadd = std::stoi(tokens[i+2].val);
        instr2 = {op,memadd};
        ++i;
        load = true;
        continue;
      }
      instr2 = {op};
      
      load = true;
      continue;
    }
  }

  std::println("instr {}",instrs);
  for (int32_t i: instrs) {
    std::println("MEM {}",binaryString(i));
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
  convertTokensToInstructions(instrs);
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
  std::print("[");
  for (const Token &t : tokens) {
    std::print("[{},{}]", (int)t.token, t.val);
  }
  std::println("]");
  convertTokensToBytes(tokens);
}
