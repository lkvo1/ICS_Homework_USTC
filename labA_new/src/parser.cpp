#include "parser.h"

void Parser::displaySymbolTable() const {
    std::cout << "Symbol Table:" << std::endl;
    for (const auto& entry : symbol_table_) {
        std::cout << entry.first << " : 0x" << std::hex << entry.second << std::dec << std::endl;
    }
}

void Parser::firstPass(const std::vector<Token>& tokens) {
    uint16_t pc = 0;    // 地址计数器
    const Token* prevToken = nullptr;
    Instruction currentInstruction = Instruction();
    for (const Token& token : tokens) {
        // 忽略逗号
        if (token.getType() == TokenType::CM) {
            continue;
        }

        // 处理开始地址伪指令
        if (token.getType() == TokenType::IMM && prevToken != nullptr && prevToken->getStrValue() == ".ORIG") {
            pc = static_cast<uint16_t>(token.getValue()) - 1; // -1 抵消下一个EOL
            continue;
        }

        // 处理标签
        if (token.getType() == TokenType::LBL && prevToken != nullptr && prevToken->getType() == TokenType::EOL) {
                addSymbol(token.getStrValue(), pc);
        }

        // 处理 .STRINGZ 伪指令
        if (token.getType() == TokenType::STR && prevToken != nullptr && prevToken->getStrValue() == ".STRINGZ") {
            pc += static_cast<uint16_t>(token.getStrValue().length()); // 分配空间
        }

        // 处理 .BLKW 伪指令
        if (token.getType() == TokenType::NUM && prevToken != nullptr && prevToken->getStrValue() == ".BLKW") {
            pc += static_cast<uint16_t>(token.getValue() - 1); // 分配空间
        }

        // 每遇到一个指令或伪指令，地址计数器加1
        if (token.getType() == TokenType::EOL) {
            pc++;
        }
        prevToken = &token;
    }

    displaySymbolTable();
}

void Parser::secondPass() {
    
}