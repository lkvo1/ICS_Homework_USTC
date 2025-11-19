#include "asmFmt.h"

void formatAssembly(std::string &asmFilePath) {
    // 对asm文件进行格式化处理, 便于tokenizer解析
    std::ifstream origAsmFile(asmFilePath);
    if (!origAsmFile.is_open()) {
        std::cerr << "Error: Unable to open assembly file." << asmFilePath << std::endl;
        return;
    }

    std::string formattedFilePath = asmFilePath + ".fmt";
    std::ofstream fmtAsmFile(formattedFilePath);
    // 打开 origAsmFile 然后将其中的内容格式化写入 fmtAsmFile
    std::string line;
    while (std::getline(origAsmFile, line)) {
        // 去除开头空格
        while (!line.empty()) {
            if (line[0] == ' ' || line[0] == '\t') line = line.substr(1);
            else break;
        }

        // 去除开头空白之后如果这一行是空的或者全是注释也直接忽略
        if (line.empty() || line[0] == ';' || line[0] == '\t') continue;
        
        bool inString = false;
        std::string proc_line;
        for (char c : line) {
            if (c == ';') break; // 注释部分不处理
            if (c == '\"') { inString = !inString; };
            if (!inString && c == ',') {
                proc_line += ' ';
                proc_line += c;
                proc_line += ' ';
                continue;
            }
            proc_line += c;
        }
        fmtAsmFile << proc_line << std::endl;
    }
}