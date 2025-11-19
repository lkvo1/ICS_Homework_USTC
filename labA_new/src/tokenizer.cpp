#include "tokenizer.h"

bool isNumType(const std::string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return !str.empty();    // 如果字符串非空并且全都是整数, 那么是NUM类型
}

bool isImmType(const std::string& str) {
    if (str.empty()) return false;
    if (str[0] == '#') {
        // 十进制
        for (int i = 1; i < str.size(); ++i) {
            if (!isdigit(str[i])) return false;
        }
        return str.size() > 1;
    } else if (str[0] == 'x' || str[0] == 'X') {
        // 十六进制
        for (int i = 1; i < str.size(); ++i) {
            if (!isxdigit(str[i])) return false;
        }
        return str.size() > 1;
    } else if (str[0] == 'B' || str[0] == 'b') {
        // 二进制
        for (int i = 1; i < str.size(); ++i) {
            if (str[i] != '0' && str[i] != '1') return false;
        }
        return str.size() > 1;
    }
    return false;
}

bool isRegType(const std::string& str) {
    if (str == "R0" || str == "R1" || str == "R2" || str == "R3" ||
        str == "R4" || str == "R5" || str == "R6" || str == "R7") {
        return true;
    }
    return false;
}

bool isOpType(const std::string& str) {
    if (str == "ADD" || str == "AND" || str == "BR" || str == "JMP" || 
        str == "JSR" || str == "JSRR" || str == "LD" || str == "LDR" || 
        str == "LEA" || str == "NOT" || str == "RET" || str == "RTI" || 
        str == "STI" || str == "ST" || str == "STR" || str == "TRAP" ||
        str == "LDI" || str == "BRn" || str == "BRz" || str == "BRp" || 
        str == "BRnz" || str == "BRnp" || str == "BRzp" || str == "BRnzp") return true;
    return false;
}

bool isPsdType(const std::string& str) {
    if (str == ".ORIG" || str == ".END" || str == "IN" || str == "OUT" || 
        str == "PUTSP" || str == ".FILL" || str == ".BLKW" || str == ".STRINGZ" ||
        str == "HALT"  || str == "IN"    || str == "OUT"   || str == "PUTS" || 
        str == "GETC" ) return true;
    return false;
}

bool isStrType(const std::string& str) {
    if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
        return true;
    }
    return false;
}

bool isLblType(const std::string& str) {
    if (str.empty()) return false;
    if (!isalpha(str[0]) && str[0] != '_') return false; // 首字符必须是字母或下划线
    for (int i = 1; i < str.size(); ++i) {
        if (!isalnum(str[i]) && str[i] != '_') return false; // 后续字符必须是字母、数字或下划线
    }
    return true;
}

TokenType to_TokenType(const std::string& token_name) {
    // 根据 token_name 返回对应的 TokenType
    if (token_name == "EOF") { return TokenType::EOF; }
    if (token_name == ",") { return TokenType::CM; }
    if (token_name == "\n") { return TokenType::EOL; }
    if (isOpType(token_name)) { return TokenType::OP; }
    if (isImmType(token_name)) { return TokenType::IMM; }
    if (isRegType(token_name)) { return TokenType::REG; }
    if (isNumType(token_name)) { return TokenType::NUM; }
    if (isPsdType(token_name)) { return TokenType::PSD; }
    if (isStrType(token_name)) { return TokenType::STR; }
    if (isLblType(token_name)) { return TokenType::LBL; }
    return TokenType::UNK;
}

uint16_t getMachineCode(const std::string& token_name) {
    // 输入 OP 或 PSD, 返回对应机器码
    if (token_name == "ADD") return 0b0001;
    if (token_name == "AND") return 0b0101;
    if (token_name.substr(0, 2) == "BR") return 0b0000;
    if (token_name == "JMP") return 0b1100;
    if (token_name == "JSR") return 0b0100;
    if (token_name == "JSRR") return 0b0100;
    if (token_name == "LD") return 0b0010;
    if (token_name == "LDR") return 0b0110;
    if (token_name == "LDI") return 0b1010;
    if (token_name == "LEA") return 0b1110;
    if (token_name == "NOT") return 0b1001;
    if (token_name == "RET") return 0b1100;
    if (token_name == "RTI") return 0b1000;
    if (token_name == "ST") return 0b0011;
    if (token_name == "STI") return 0b1011;
    if (token_name == "STR") return 0b0111;
    if (token_name == "TRAP") return 0b1111;
    return 0b1101;  // reversed
}

std::string to_string(TokenType type) {
    switch (type) {
        case TokenType::OP: return "OP";
        case TokenType::IMM: return "IMM";
        case TokenType::REG: return "REG";
        case TokenType::CM: return "CM";
        case TokenType::EOF: return "EOF";
        case TokenType::EOL: return "EOL";
        case TokenType::LBL: return "LBL";
        case TokenType::PSD: return "PSD";
        case TokenType::NUM: return "NUM";
        case TokenType::STR: return "STR";
        default: return "UNK";
    }
}

void Token::setToken(const std::string &token_name) {
    // 根据 token_name 设置 Token 的各项成员
    // type
    type_ = to_TokenType(token_name);

    // str_value
    if (type_ == TokenType::STR) { str_value_ = token_name.substr(1, token_name.size() - 2); } // 去掉左右的引号
    else str_value_ = token_name;

    // value
    if (type_ == TokenType::IMM) {
        switch (token_name[0]) {
            case '#': value_ = static_cast<uint16_t>(std::stoi(token_name.substr(1))); break;
            case 'x':
            case 'X': value_ = static_cast<uint16_t>(std::stoul(token_name.substr(1), nullptr, 16)); break;
            case 'b':
            case 'B': value_ = static_cast<uint16_t>(std::stoul(token_name.substr(1), nullptr, 2)); break;
        }
    }
    else if (type_ == TokenType::REG) { value_ = static_cast<uint16_t>(token_name[1] - '0'); }
    else if (type_ == TokenType::NUM) { value_ = static_cast<uint16_t>(std::stoi(token_name)); }
    else if (type_ == TokenType::OP || type_ == TokenType::PSD) { value_ = getMachineCode(token_name); }
    else { value_ = 0; }
}

std::vector<Token> tokenize(const std::string &formattedFilePath) {
    std::vector<Token> tokens;  // 存储解析得到的Tokens
    std::ifstream fmtAsmFile(formattedFilePath);    // 打开格式化后的asm
    if (!fmtAsmFile.is_open()) {
        std::cerr << "Error: Unable to open formatted assembly file." << formattedFilePath << std::endl;
        return tokens;
    }
    // 逐个读取token
    std::string line;
    std::string token_name;
    Token* prev_Token = nullptr;
    while (getline(fmtAsmFile, line)) {
        std::istringstream lineStream(line);
        while (lineStream >> token_name) {
            // 对于字符串类型, 需要处理空格, 这里使用while循环读取直到遇到结束引号               
            // 遇到结束引号后跳出循环, token_name即为完整字符串
            if (token_name.front() == '\"') {
                char ch;
                while ((token_name.back() != '\"' || token_name.size() == 1)) {
                    if (lineStream.get(ch)) { token_name += ch; }
                    else { std::cerr << "Error: Cannot find ending quote for a string." << std::endl; exit(1); }
                }
            }
            Token token = Token();
            token.setToken(token_name);
            tokens.push_back(token);
        }
        // 如果本行只有一个LBL, 那么不能添加EOL, 否则每行结束后添加 EOL token
        if (!(tokens.back().getType() == TokenType::LBL && tokens.size() >= 2 &&
            tokens[tokens.size() - 2].getType() == TokenType::EOL)) {
            Token eol_token;
            eol_token.setToken("\n");
            tokens.push_back(eol_token);
        }
    }
    // 添加 EOF
    Token eof_token;
    eof_token.setToken("EOF");
    tokens.push_back(eof_token);

    // debug: 将所有token写入asmToken.txt
    std::ofstream debugFile("asmToken.txt");
    for (const Token& token : tokens) {
        debugFile << "Token Type: " << to_string(token.getType()) 
                  << ", Value: " <<  token.getValue() 
                  << ", StrValue: " <<  token.getStrValue() << std::endl;
    }
    debugFile.close();

    return tokens;
}