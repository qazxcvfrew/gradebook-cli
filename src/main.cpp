// main.cpp —— gradebook-cli 命令行入口
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "gradebook.h"

namespace {

std::string readFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        return "";
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

void printUsage(const char* program) {
    std::cout << "usage: " << program << " [scores.csv]\n"
              << "  scores.csv 形如：\n"
              << "    name,math,english\n"
              << "    lisi,88,92\n"
              << "  省略路径时默认读取 data/scores.csv\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc > 1) {
        const std::string first = argv[1];
        if (first == "-h" || first == "--help") {
            printUsage(argv[0]);
            return 0;
        }
    }

    const std::string path = (argc > 1) ? argv[1] : "data/scores.csv";
    const std::string text = readFile(path);
    if (text.empty()) {
        std::cerr << "cannot read scores from: " << path << "\n";
        printUsage(argv[0]);
        return 1;
    }

    const std::vector<gradebook::Student> students = gradebook::parseCSV(text);
    if (students.empty()) {
        std::cerr << "no valid student record found in: " << path << "\n";
        return 1;
    }

    std::cout << gradebook::formatReport(students);
    return 0;
}
