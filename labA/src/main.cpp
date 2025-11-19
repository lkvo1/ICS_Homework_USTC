#include <iostream>
#include <fstream>
#include "tokenizer.h"
#include "parser.h"
extern std::vector<std::string> sb; // debug use
int main() {
    std::ifstream file("lab5.asm");
    std::vector<Token> tokens = tokenizer(file);
    // for (const auto& token : tokens) {
    //     std::cout << "Token Type: " << to_string(token.type()) << ", Value: " << token.value() << ", " << sb.front() << std::endl;
    //     sb.erase(sb.begin());
    // }

    Parser parser;
    parser.firstPass(tokens);
    parser.secondPass(tokens);
    // parser.displaySymbolTable();
    return 0;
}