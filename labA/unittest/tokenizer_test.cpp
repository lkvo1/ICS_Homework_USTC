#include <iostream>
#include <fstream>
#include "tokenizer.h"
int main() {
    std::ifstream file("lab5.asm");
    std::vector<Token> tokens = tokenizer(file);
    for (const auto& token : tokens) {
        std::cout << "Token Type: " << to_string(token.type()) << ", Value: " << token.value() << std::endl;
    }
    return 0;
}