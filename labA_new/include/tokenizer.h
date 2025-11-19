#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#undef EOF
// 定义 Token 类型
enum class TokenType {
    OP, IMM, REG, CM, EOF, EOL, LBL, PSD, NUM, STR, UNK
};

// 定义 Token 类
class Token {
private:
    TokenType type_;
    uint16_t value_; // 存储器编号或立即数的值
    std::string str_value_; // 存储字符串值 (对于其他Token直接存储Token本身, 如果Token是字符串则存储字符串的值)

public:
    Token(TokenType type = TokenType::UNK, uint16_t val = 0, const std::string &str_val = "") : type_(type), value_(val), str_value_(str_val) {}
    TokenType getType() const { return type_; }
    uint16_t getValue() const { return value_; }
    std::string getStrValue() const { return str_value_; }
    void setToken(const std::string &token_name);
};

std::vector<Token> tokenize(const std::string &formattedFilePath);

#endif // TOKENIZER_H