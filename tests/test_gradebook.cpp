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

void testMedian() {
    checkClose(gradebook::median({}), 0.0, "median 对空成绩返回 0");
    checkClose(gradebook::median({90}), 90.0, "median 奇数个成绩取中间值");
    checkClose(gradebook::median({80, 90, 100}), 90.0, "median 奇数个成绩取中间值（乱序输入）");
    checkClose(gradebook::median({80, 90}), 85.0, "median 偶数个成绩取中间两个的平均");
    checkClose(gradebook::median({70, 100, 80, 90}), 85.0, "median 偶数个成绩取中间两个的平均（乱序输入）");
}

void testPassRate() {
    checkClose(gradebook::passRate({}), 0.0, "passRate 对空成绩返回 0（不除零）");
    checkClose(gradebook::passRate({59, 60, 100}), 2.0 / 3.0, "passRate 按及格线统计占比");
    checkClose(gradebook::passRate({50, 40}, 50), 0.5, "passRate 支持自定义及格线");
}

void testLetterGrade() {
    check(gradebook::letterGrade(95.0) == 'A', "letterGrade 95 分为 A");
    check(gradebook::letterGrade(90.0) == 'A', "letterGrade 90 分（下界）为 A");
    check(gradebook::letterGrade(89.9) == 'B', "letterGrade 89.9 分为 B");
    check(gradebook::letterGrade(80.0) == 'B', "letterGrade 80 分（下界）为 B");
    check(gradebook::letterGrade(70.0) == 'C', "letterGrade 70 分（下界）为 C");
    check(gradebook::letterGrade(60.0) == 'D', "letterGrade 60 分（下界）为 D");
    check(gradebook::letterGrade(59.9) == 'F', "letterGrade 59.9 分为 F");
}

void testGradeOf() {
    const gradebook::Student mixed{"lisi", {95, 40}};  // 平均 67.5
    check(gradebook::gradeOf(mixed) == 'D', "gradeOf 按平均分评定等级，而不是首次成绩");

    const gradebook::Student empty{"zhaoliu", {}};
    check(gradebook::gradeOf(empty) == 'F', "gradeOf 对空成绩返回 F（不越界访问）");

    const gradebook::Student top{"zhangsan", {95, 91}};  // 平均 93
    check(gradebook::gradeOf(top) == 'A', "gradeOf 平均 93 分为 A");
}

void testFormatReportWithStats() {
    const std::vector<gradebook::Student> students = {{"lisi", {80, 90}}};
    const std::string plain = gradebook::formatReport(students);
    const std::string stats = gradebook::formatReport(students, true);
    const std::string graded = gradebook::formatReport(students, false, true);
    check(plain.find("median") == std::string::npos, "formatReport 默认不输出统计信息");
    check(stats.find("median=85") != std::string::npos, "formatReport 在 withStats 时输出中位数");
    check(graded.find("grade=B") != std::string::npos, "formatReport 在 withGrade 时输出等级");
}

}  // namespace

int main() {
    testParseCSV();
    testAverage();
    testMaxScore();
    testMedian();
    testPassRate();
    testLetterGrade();
    testGradeOf();
    testFormatReportWithStats();

    if (g_failures == 0) {
        std::cout << "\nall tests passed\n";
        return 0;
    }
    std::cout << "\n" << g_failures << " test(s) failed\n";
    return 1;
}
