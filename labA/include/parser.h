#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <iostream>

class Instruction {
public:
    void addToken(const Token& token) {
        ins.push_back(token);
    }
    
    void clear() {
        ins.clear();
        code = 0;
    }
    
    uint16_t getCode() const {
        return code;
    }

    // 设置当前指令地址和符号表，以便计算 offset
    void setContext(uint16_t addr, const std::map<std::string, std::uint16_t>& symTable) {
        currentAddress = addr;
        symbolTablePtr = &symTable;
    }

    // 兼容旧代码的接口，但在新逻辑中可能不再主要使用
    void setCurrentAddress() {}

    void assembly();

private:
    std::vector<Token> ins;
    uint16_t code = 0;
    uint16_t currentAddress = 0;
    const std::map<std::string, std::uint16_t>* symbolTablePtr = nullptr;

    // 辅助函数，用于模块化
    void processAddAndNot();
    void processLoadStore(); // LD, LDI, LEA, ST, STI
    void processBaseOffset(); // LDR, STR
    void processBranch();
    void processJmpJsrr();
    void processJsr();
    void processTrap();
    
    // 辅助计算 offset
    int16_t getOffset(const std::string& label, int bits);
};

class Parser {
public:
    void addSymbol(const std::string& symbol, std::uint16_t addr);
    void firstPass(const std::vector<Token>& tokens);
    void secondPass(const std::vector<Token>& tokens);
    void displaySymbolTable();

private:
    std::map<std::string, std::uint16_t> symbolTable_;
    std::uint16_t count_ = 0;
};

#endif // PARSER_H
