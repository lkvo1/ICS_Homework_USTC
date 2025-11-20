#include "asmFmt.h"
#include "instruction.h"
#include "parser.h"
#include "tokenizer.h"
#include "CLI11.hpp"

int main(int argc, char** argv) {
    CLI::App app{"LC-3 Assembler"};

    std::string asmFilePath;
    std::string outputFilePath = "output.bin";

    app.add_option("-i,--input", asmFilePath, "Input assembly file")
        ->required()
        ->check(CLI::ExistingFile);
    
    app.add_option("-o,--output", outputFilePath, "Output machine code file");

    CLI11_PARSE(app, argc, argv);

    // 格式化汇编文件
    formatAssembly(asmFilePath);

    std::string formattedFilePath = asmFilePath + ".fmt";

    // tokenization
    std::vector<Token> tokens = tokenize(formattedFilePath);

    // parsing
    Parser parser;
    parser.firstPass(tokens);
    parser.secondPass();
    parser.assemble(outputFilePath);
    return 0;
}