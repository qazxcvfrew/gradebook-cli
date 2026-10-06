// gradebook.cpp —— 成绩簿核心实现
#include "gradebook.h"

#include <algorithm>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

namespace gradebook {

namespace {

// 按分隔符切分；不保留空字段之外的特殊处理。
std::vector<std::string> split(const std::string& line, char delim) {
    std::vector<std::string> parts;
    std::string current;
    for (char ch : line) {
        if (ch == delim) {
            parts.push_back(current);
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    parts.push_back(current);
    return parts;
}

// 去掉首尾空白（含 \r，兼容 Windows 行尾）。
std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n";
    const std::size_t begin = text.find_first_not_of(whitespace);
    if (begin == std::string::npos) {
        return "";
    }
    const std::size_t end = text.find_last_not_of(whitespace);
    return text.substr(begin, end - begin + 1);
}

}  // namespace

std::vector<Student> parseCSV(const std::string& text) {
    std::vector<Student> students;
    std::istringstream stream(text);
    std::string line;

    while (std::getline(stream, line)) {
        const std::string trimmed = trim(line);
        if (trimmed.empty()) {
            continue;
        }
        const std::vector<std::string> fields = split(trimmed, ',');
        const std::string name = trim(fields.front());
        if (name.empty() || name == "name") {  // 跳过表头
            continue;
        }

        Student student;
        student.name = name;
        for (std::size_t i = 1; i < fields.size(); ++i) {
            const std::string value = trim(fields[i]);
            if (value.empty()) {
                continue;
            }
            student.scores.push_back(std::stoi(value));
        }
        students.push_back(student);
    }

    return students;
}

double average(const std::vector<int>& scores) {
    if (scores.empty()) {
        return 0.0;
    }
    int sum = 0;
    for (int score : scores) {
        sum += score;
    }
    return static_cast<double>(sum) / static_cast<double>(scores.size());
}

int maxScore(const std::vector<int>& scores) {
    if (scores.empty()) {
        return 0;
    }
    int best = scores.front();
    for (int score : scores) {
        if (score > best) {
            best = score;
        }
    }
    return best;
}

double median(const std::vector<int>& scores) {
    if (scores.empty()) {
        return 0.0;
    }
    std::vector<int> sorted = scores;
    std::sort(sorted.begin(), sorted.end());
    const std::size_t middle = sorted.size() / 2;
    if (sorted.size() % 2 == 1) {
        return static_cast<double>(sorted[middle]);
    }
    // 偶数个成绩：取中间两个数的平均值
    return (static_cast<double>(sorted[middle - 1]) + static_cast<double>(sorted[middle])) / 2.0;
}

double passRate(const std::vector<int>& scores, int passMark) {
    if (scores.empty()) {
        return 0.0;
    }
    int passed = 0;
    for (int score : scores) {
        if (score >= passMark) {
            ++passed;
        }
    }
    return static_cast<double>(passed) / static_cast<double>(scores.size());
}

std::string formatReport(const std::vector<Student>& students, bool withStats) {
    std::ostringstream out;
    for (const Student& student : students) {
        out << student.name << ": average=" << average(student.scores)
            << ", max=" << maxScore(student.scores);
        if (withStats) {
            out << ", median=" << median(student.scores)
                << ", passRate=" << passRate(student.scores);
        }
        out << "\n";
    }
    return out.str();
}

}  // namespace gradebook
