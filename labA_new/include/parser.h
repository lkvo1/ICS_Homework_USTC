#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "instruction.h"
#include <vector>
#include <map>

class Parser {
private:
    std::vector<Instruction> instructions_;
    std::map<std::string, uint16_t> symbol_table_;  // 标签符号表, 用于计算offset
    std::vector<uint16_t> machineCode_;              // 最终生成的机器码

public:
    
    void firstPass(const std::vector<Token>& tokens);   // 第一遍扫描，构建符号表
    void secondPass(const std::vector<Token>& tokens);  // 第二遍扫描，生成指令列表
    void addInstruction(const Instruction& ins) { instructions_.push_back(ins); }   // 添加指令
    std::vector<Instruction> getInstructions() const { return instructions_; }       // 获取指令列表
    void assemble();    // 汇编生成机器码
    void addSymbol(std::string symbol, uint16_t address) { symbol_table_[symbol] = address; }
    void displaySymbolTable() const;
};



#endif // PARSER_H