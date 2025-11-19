#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include "tokenizer.h"

class Instruction {
private:
    std::vector<Token> tokens_;
    uint16_t address_;

public:
    Instruction() : address_(0) {}
    Instruction(std::vector<Token> tokens, uint16_t addr) : tokens_(tokens), address_(addr) {}
    void addToken(const Token& token) {tokens_.push_back(token); }
    std::vector<Token> getTokens() const { return tokens_; }
    void setAddress(uint16_t addr) { address_ = addr; }
};



#endif // INSTRUCTION_H