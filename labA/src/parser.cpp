#include "parser.h"
#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>

void Parser::displaySymbolTable() const {
    std::cout << "Symbol Table:" << std::endl;
    for (const auto& entry : symbol_table_) {
        std::cout << entry.first << " : 0x" << std::hex << entry.second << std::dec << std::endl;
    }
}

void Parser::displayInstructions() const {
    std::cout << "Instructions:" << std::endl;
    for (const auto& ins : instructions_) {
        std::cout << "Address: 0x" << std::hex << ins.getAddress() << std::dec << " Tokens: ";
        for (const auto& token : ins.getTokens()) {
            std::cout << token.getStrValue() << " ";
        }
        std::cout << std::endl;
    }
}

void Parser::firstPass(const std::vector<Token>& tokens) {
    uint16_t pc = 0;    // 地址计数器
    bool blkwMode = false, strzMode = false, origMode = false;
    const Token* prevToken = nullptr;
    Instruction currentInstruction = Instruction();
    for (const Token& token : tokens) {
        // 忽略逗号
        if (token.getType() == TokenType::CM) {
            continue;
        }

        // 处理开始地址伪指令
        if (token.getType() == TokenType::IMM && prevToken != nullptr && prevToken->getStrValue() == ".ORIG") {
            origMode = true;
            pc = static_cast<uint16_t>(token.getValue()) - 1; // -1 抵消下一个EOL
        }

        // 处理标签
        if (token.getType() == TokenType::LBL && prevToken != nullptr && prevToken->getType() == TokenType::EOL) {
            addSymbol(token.getStrValue(), pc);
        }

        // 处理 .STRINGZ 伪指令
        if (token.getType() == TokenType::STR && prevToken != nullptr && prevToken->getStrValue() == ".STRINGZ") {
            strzMode = true;
        }

        // 处理 .BLKW 伪指令
        if (token.getType() == TokenType::NUM && prevToken != nullptr && prevToken->getStrValue() == ".BLKW") {
            blkwMode = true;
        }

        // 将token添加到当前指令
        currentInstruction.addToken(token);

        // 每遇到一个EOL, 表示当前指令已经装填完毕, 地址计数器加1, 下一次循环处理下一个指令
        if (token.getType() == TokenType::EOL) {
            // 此时指令已经完整, 设置地址并保存到指令表中, 然后重置当前指令
            if (origMode) { currentInstruction.setAddress(pc + 1); origMode = false; } 
            else currentInstruction.setAddress(pc);
            addInstruction(currentInstruction);
            currentInstruction = Instruction();

            // 根据伪指令触发模式修正pc
            if (strzMode) { pc += static_cast<uint16_t>(prevToken->getStrValue().length() + 1 - 1); strzMode = false; } // 分配空间, +1 表示尾部结束符, -1 抵消EOL
            if (blkwMode) { pc += static_cast<uint16_t>(prevToken->getValue() - 1); blkwMode = false; } // 分配空间
            // 遇到EOL地址计数器仍然 +1, 不同模式已经添加修正值
            pc++;
        }
        prevToken = &token;
    }

    displaySymbolTable();
    displayInstructions();
}

// 根据指令列表和符号表生成最终的机器码
void Parser::secondPass() {
    for (Instruction& ins : instructions_) {
        // 根据指令生成对应的机器码, 首先去除可能的Label
        if (ins.getTokens()[0].getType() == TokenType::LBL) ins.removeLabel();
        // 根据指令类型生成机器码
        switch (ins.getTokens()[0].getType()) {
            // 单独在此处处理stringz 和 blwk 伪指令
            case TokenType::OP:
            case TokenType::PSD:    // Pseudo-opcode
                if (ins.getTokens()[0].getStrValue() == ".STRINGZ") {
                    // 逐字符转换为机器码
                    const std::string& str = ins.getTokens()[1].getStrValue();
                    for (char c : str) {
                        std::cout <<static_cast<uint16_t>(c) << '\t' <<std::bitset<16>(static_cast<uint16_t>(c)) << ' ' << c << std::endl;
                        machineCode_.push_back(static_cast<uint16_t>(c));
                    }
                    // 添加结尾的 null 字符
                    machineCode_.push_back(0); // 结尾的 \0 字符
                    std::cout <<static_cast<uint16_t>(0) << '\t' <<std::bitset<16>(static_cast<uint16_t>(0)) << " \\0" << std::endl;
                } else if (ins.getTokens()[0].getStrValue() == ".BLKW") {
                    uint16_t numWords = ins.getTokens()[1].getValue();
                    for (uint16_t i = 0; i < numWords; ++i) {
                        std::cout <<static_cast<uint16_t>(0) << '\t' <<std::bitset<16>(static_cast<uint16_t>(0)) << " \\0" << std::endl;
                        machineCode_.push_back(0); // 分配的空间初始化为0
                    }
                } // else if (ins.getTokens()[0].getStrValue() == ".ORIG") {
                    // continue;} 
                else if (ins.getTokens()[0].getStrValue() == ".END") { 
                    break; // 汇编结束
                } else {
                    machineCode_.push_back(toMachineCode(ins));
                }
                break;
            default:
                std::cerr << "Error: Syntax error in instruction at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
                break;
        }
    }
}

uint16_t Parser::toMachineCode(const Instruction& ins) {
    uint16_t mCode = 0;
    std::vector<Token> tokens = ins.getTokens();
    // Tokens[0]只有两种可能性 OP or PSD, 按照对应的规则生成机器码填写到 mCode 的高4位
    if (tokens[0].getStrValue() == "ADD") {
        mCode |= (0b0001 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // DR
        mCode |= (tokens[2].getValue() & 0b111) << 6; // SR1
        if (tokens[3].getType() == TokenType::REG) {
            mCode |= (tokens[3].getValue() & 0b111); // SR2
        } else if (tokens[3].getType() == TokenType::IMM) {
            mCode |= (1 << 5); // 立即数标志位
            mCode |= (tokens[3].getValue() & 0b11111); // 立即数值
        } else {
            std::cerr << "Error: Syntax error in ADD instruction at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
        }
    }
    if (tokens[0].getStrValue() == "AND") {
        mCode |= (0b0101 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // DR
        mCode |= (tokens[2].getValue() & 0b111) << 6; // SR1
        if (tokens[3].getType() == TokenType::REG) {
            mCode |= (tokens[3].getValue() & 0b111); // SR2
        } else if (tokens[3].getType() == TokenType::IMM) {
            mCode |= (1 << 5); // 立即数标志位
            mCode |= (tokens[3].getValue() & 0b11111); // 立即数值
        } else {
            std::cerr << "Error: Syntax error in AND instruction at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
        }
    }
    if (tokens[0].getStrValue().substr(0, 2) == "BR") {
        mCode |= (0b0000 << 12);    // opcode
        // nzp bits
        bool n = false, z = false, p =false;
        if (tokens[0].getStrValue().find('n') != std::string::npos) n = true;
        if (tokens[0].getStrValue().find('z') != std::string::npos) z = true;
        if (tokens[0].getStrValue().find('p') != std::string::npos) p = true;
        if (n) mCode |= (1 << 11);
        if (z) mCode |= (1 << 10);
        if (p) mCode |= (1 << 9);
        if (!n && !z && !p) {
            // 全部不设置则表示 nzp 全部置1
            mCode |= (0b111 << 9);
        }
        // PCoffset9   pc + 1 + offset = tgt_addr -> offset = tft_addr - pc - 1
        int16_t offset = static_cast<int16_t>(symbol_table_[tokens[1].getStrValue()]) - static_cast<int16_t>(ins.getAddress()) - 1;
        if (offset < -256 || offset > 255) {
            std::cerr << "Error: BR offset out of range at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
        } else {
            mCode |= (offset & 0x1FF);
        }
    }
    if (tokens[0].getStrValue() == "JMP") {
        mCode |= (0b1100 << 12);
        mCode |= (0b000 << 9);
        mCode |= (tokens[1].getValue() & 0b111) << 6; // BaseR
        mCode |= 0b000000;
    }
    if (tokens[0].getStrValue() == "JSR") {
        mCode |= (0b0100 << 12);
        mCode |= (0b1 << 11);
        // PCoffset11   pc + 1 + offset = tgt_addr -> offset = tgt_addr - pc - 1
        int16_t offset = static_cast<int16_t>(symbol_table_[tokens[1].getStrValue()]) - static_cast<int16_t>(ins.getAddress()) - 1;
        if (offset < -1024 || offset > 1023) {
            std::cerr << "Error: JSR offset out of range at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
        } else {
            mCode |= (offset & 0x7FF);
        }
    }
    if (tokens[0].getStrValue() == "JSRR") {
        mCode |= (0b0100 << 12);
        mCode |= (0b000 << 9);
        mCode |= (tokens[1].getValue() & 0b111) << 6; // BaseR
        mCode |= 0b000000;
    }
    if (tokens[0].getStrValue() == "LD") {
        mCode |= (0b0010 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // DR
        // PCoffset9   pc + 1 + offset = tgt_addr -> offset = tgt_addr - pc - 1
        int16_t offset = static_cast<int16_t>(symbol_table_[tokens[2].getStrValue()]) - static_cast<int16_t>(ins.getAddress()) - 1;
        if (offset < -256 || offset > 255) {
            std::cerr << "Error: LD offset out of range at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
        } else {
            mCode |= (offset & 0x1FF);
        }
    }
    if (tokens[0].getStrValue() == "LDR") {
        mCode |= (0b0110 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // DR
        mCode |= (tokens[2].getValue() & 0b111) << 6; // BaseR
        // PCoffset6 将立即数转换为6位二进制补码
        uint8_t offset = static_cast<uint8_t>(tokens[3].getValue());
        mCode |= (offset & 0x3F);
    }
    if (tokens[0].getStrValue() == "LEA") {
        mCode |= (0b1110 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // DR
        // PCoffset9   pc + 1 + offset = tgt_addr -> offset = tgt_addr - pc - 1
        // debug:
        // std::cout << "LEA offset calculation: symbol address = " << symbol_table_[tokens[2].getStrValue()] << ", instruction address = " << ins.getAddress() << std::endl;
        int16_t offset = static_cast<int16_t>(symbol_table_[tokens[2].getStrValue()]) - static_cast<int16_t>(ins.getAddress()) - 1;
        if (offset < -256 || offset > 255) {
            std::cerr << "Error: LEA offset out of range at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
        } else {
            mCode |= (offset & 0x1FF);
        }
    }
    if (tokens[0].getStrValue() == "NOT") {
        mCode |= (0b1001 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // DR
        mCode |= (tokens[2].getValue() & 0b111) << 6; // SR
        mCode |= 0b111111;
    }
    if (tokens[0].getStrValue() == "RET") {
        mCode |= (0b1100000111000000);
    }
    if (tokens[0].getStrValue() == "RTI") {
        mCode |= (0b1000000000000000);
    }
    if (tokens[0].getStrValue() == "ST") {
        mCode |= (0b0011 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // SR
        // PCoffset9   pc + 1 + offset = tgt_addr -> offset = tgt_addr - pc - 1
        int16_t offset = static_cast<int16_t>(symbol_table_[tokens[2].getStrValue()]) - static_cast<int16_t>(ins.getAddress()) - 1;
        if (offset < -256 || offset > 255) {
            std::cerr << "Error: ST offset out of range at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
        } else {
            mCode |= (offset & 0x1FF);
        }
    }
    if (tokens[0].getStrValue() == "STI") {
        mCode |= (0b1011 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // SR
        // PCoffset9   pc + 1 + offset = tgt_addr -> offset = tgt_addr - pc - 1
        int16_t offset = static_cast<int16_t>(symbol_table_[tokens[2].getStrValue()]) - static_cast<int16_t>(ins.getAddress()) - 1;
        if (offset < -256 || offset > 255) {
            std::cerr << "Error: STI offset out of range at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
        } else {
            mCode |= (offset & 0x1FF);
        }
    }
    if (tokens[0].getStrValue() == "STR") {
        mCode |= (0b0111 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // SR
        mCode |= (tokens[2].getValue() & 0b111) << 6; // BaseR
        // PCoffset6 将立即数转换为6位二进制补码
        uint8_t offset = static_cast<uint8_t>(tokens[3].getValue());
        mCode |= (offset & 0x3F);
    }
    if (tokens[0].getStrValue() == "TRAP") {
        mCode |= (0b1111 << 12);
        mCode |= (tokens[1].getValue() & 0xFF); // trapvect8
    }
    if (tokens[0].getStrValue() == "LDI") {
        mCode |= (0b1010 << 12);
        mCode |= (tokens[1].getValue() & 0b111) << 9; // DR
        // PCoffset9   pc + 1 + offset = tgt_addr -> offset = tgt_addr - pc - 1
        int16_t offset = static_cast<int16_t>(symbol_table_[tokens[2].getStrValue()]) - static_cast<int16_t>(ins.getAddress()) - 1;
        if (offset < -256 || offset > 255) {
            std::cerr << "Error: LDI offset out of range at address 0x" << std::hex << ins.getAddress() << std::dec << std::endl;
        } else {
            mCode |= (offset & 0x1FF);
        }
    }
    if (tokens[0].getStrValue() == "GETC") {
        mCode |= (0b1111 << 12);
        mCode |= 0x0020;
    }
    if (tokens[0].getStrValue() == "OUT") {
        mCode |= (0b1111 << 12);
        mCode |= 0x0021;
    }
    if (tokens[0].getStrValue() == "PUTS") {
        mCode |= (0b1111 << 12);
        mCode |= 0x0022;
    }
    if (tokens[0].getStrValue() == "IN") {
        mCode |= (0b1111 << 12);
        mCode |= 0x0023;
    }
    if (tokens[0].getStrValue() == "PUTSP") {
        mCode |= (0b1111 << 12);
        mCode |= 0x0024;
    }
    if (tokens[0].getStrValue() == "HALT") {
        mCode |= (0b1111 << 12);
        mCode |= 0x0025;
    }
    if (tokens[0].getStrValue() == ".FILL") {
        mCode |= (tokens[1].getValue() & 0xFFFF);
    }
    // 注意此处处理.ORIG伪指令, 直接将地址写入机器码, 因为不知道lc3tools所需的obj文件格式, 因此将.ORIG的地址写入文件, 这样输出的bin文件才能运行。
    if (tokens[0].getStrValue() == ".ORIG") {
        mCode |= (tokens[1].getValue() & 0xFFFF);
    }
    //Stringz 和 Blkw 需要生成多条机器码, 需要在外部处理, 而且.END和.ORIG不生成机器码
    // debug: 打印mCode
    std::cout << static_cast<uint16_t>(mCode) << "\t" << std::bitset<16>(mCode) << std::endl;
    return mCode;
}

void Parser::assemble(const std::string& outputFilePath) {
    std::ofstream outfile(outputFilePath);

    if (!outfile) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }

    // 获取起始地址
    uint16_t start_address = 0;
    if (!instructions_.empty()) {
        start_address = instructions_[0].getAddress();
        std::cout << "Start Address: 0x" << std::hex << start_address << std::dec << std::endl;
    }
    for (int i = 0; i < machineCode_.size(); ++i) {
        // outfile  << std::hex << std::uppercase << (start_address + i) << std::bitset<16>(machineCode_[i]);
        outfile << std::bitset<16>(machineCode_[i]) << '\n';
        std::cout << 'x' << std::hex << std::uppercase << (i == 0 ? start_address : start_address + i - 1) << ' '<< std::bitset<16>(machineCode_[i]) << std::dec << std::endl;
    }

    outfile.close();
    std::cout << "Assembly completed. Output written to " << outputFilePath << std::endl;
}