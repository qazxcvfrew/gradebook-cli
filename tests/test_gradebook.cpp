// test_gradebook.cpp —— 无第三方依赖的断言式单元测试
//
// 运行：./build/tests   （全部通过时退出码为 0）
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "../src/gradebook.h"

namespace {

int g_failures = 0;

void check(bool condition, const std::string& what) {
    if (condition) {
        std::cout << "[PASS] " << what << "\n";
    } else {
        std::cout << "[FAIL] " << what << "\n";
        ++g_failures;
    }
}

void checkClose(double actual, double expected, const std::string& what) {
    const double diff = std::fabs(actual - expected);
    check(diff < 1e-9, what + " (actual=" + std::to_string(actual) + ")");
}

void testParseCSV() {
    const std::string text =
        "name,math,english\n"
        "lisi,88,92\n"
        "\n"
        "wangwu,70,80\n";
    const std::vector<gradebook::Student> students = gradebook::parseCSV(text);
    check(students.size() == 2, "parseCSV 跳过表头与空行，得到 2 名学生");
    if (students.size() == 2) {
        check(students[0].name == "lisi", "parseCSV 解析姓名");
        check(students[0].scores.size() == 2, "parseCSV 解析多列成绩");
    }
}

void testAverage() {
    checkClose(gradebook::average({80, 90}), 85.0, "average 计算平均分");
    checkClose(gradebook::average({}), 0.0, "average 对空成绩返回 0");
}

void testMaxScore() {
    check(gradebook::maxScore({70, 95, 88}) == 95, "maxScore 取最大值");
    check(gradebook::maxScore({}) == 0, "maxScore 对空成绩返回 0");
}

}  // namespace

int main() {
    testParseCSV();
    testAverage();
    testMaxScore();

    if (g_failures == 0) {
        std::cout << "\nall tests passed\n";
        return 0;
    }
    std::cout << "\n" << g_failures << " test(s) failed\n";
    return 1;
}
