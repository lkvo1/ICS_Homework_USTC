#include "asmFmt.h"
#include "instruction.h"
#include "parser.h"
#include "tokenizer.h"

int main() {
    // 格式化汇编文件
    std::string asmFilePath = "example.asm";
    formatAssembly(asmFilePath);

    std::string formattedFilePath = asmFilePath + ".fmt";

    // tokenization
    std::vector<Token> tokens = tokenize(formattedFilePath);

    // parsing
    Parser parser;
    parser.firstPass(tokens);
    // parser.secondPass(tokens);
    // parser.assemble();
    return 0;
}