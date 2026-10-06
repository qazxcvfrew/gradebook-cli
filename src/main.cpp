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
    std::cout << "usage: " << program << " [scores.csv] [--stats] [--grade]\n"
              << "  scores.csv 形如：\n"
              << "    name,math,english\n"
              << "    lisi,88,92\n"
              << "  省略路径时默认读取 data/scores.csv\n"
              << "  --stats    额外输出中位数与及格率\n"
              << "  --grade    额外输出等级（A/B/C/D/F）\n";
}

}  // namespace

int main(int argc, char** argv) {
    std::string path = "data/scores.csv";
    bool withStats = false;
    bool withGrade = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        if (arg == "--stats") {
            withStats = true;
            continue;
        }
        if (arg == "--grade") {
            withGrade = true;
            continue;
        }
        path = arg;
    }

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

    std::cout << gradebook::formatReport(students, withStats, withGrade);
    return 0;
}
