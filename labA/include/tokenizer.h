#ifndef TOKENIZER_H
#define TOKENIZER_H

#define ORIG_VALUE 1145
#define END_VALUE 19198
#define STRINGZ_VALUE 12345
#define BLKW_VALUE 54321

#include <string>
#include <tuple>
#include <ostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#undef EOF

// define token types
enum class TokenType {
    OP, IMM, REG, CM, EOF, EOL, LBL, PSD, NUM, STR, UNK
    // Represents Opcode, Immediate, Register, Comma, End of Line, End, Lable, Pseodu, Number, String and Unknown
};

enum mCode {
    ADD = 0b0001,
    AND = 0b0101,
    NOT = 0b1001,
    BR  = 0b0000,
    RET = 0b1100,
    JMP = 0b1100,
    JSR = 0b0100,
    JSRR= 0b0100,
    LD  = 0b0010,
    LDI = 0b1010,
    LDR = 0b0110,
    LEA = 0b1110,
    ST  = 0b0011,
    STI = 0b1011,
    STR = 0b0111,
    RTI = 0b1000,
    TRAP= 0b1111
};

class Token {
private:
    TokenType tokenType_;
    std::int16_t value_;
    std::string label_;
// char const* 表示指向常量字符的指针, 指针指向的内容不能更改, 但指针本身可以更改指向
public:
    Token() : tokenType_(TokenType::UNK), value_(-1) {}
    Token(TokenType type, std::uint16_t value) : tokenType_(type), value_(value) {}
    auto type() const -> TokenType { return tokenType_; }
    auto value() const -> std::uint16_t { return value_; }
    auto empty() const -> bool { return value_ == 0; }
    auto setLabel(const std::string label) -> void { label_ = label; }
    auto label() const -> std::string { return label_; }
    auto setValue(const std::int16_t value) -> void { value_ = value; }
    friend auto operator==(Token const& tokenA, Token const& tokenB) -> bool {
        return std::tie(tokenA.tokenType_, tokenA.value_) ==
               std::tie(tokenB.tokenType_, tokenB.value_);
    }

    friend auto operator!=(Token const& tokenA, Token const& tokenB) -> bool {
        return !(tokenA == tokenB);
    }
};

auto tokenTyper(std::string const& str) ->TokenType;
auto to_string(TokenType type) -> std::string;
auto tokenValuer(std::string const& str, TokenType type) -> std::int16_t;
auto tokenizer(std::ifstream& asmFile) -> std::vector<Token>;

#endif // TOKENIZER_H