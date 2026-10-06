// gradebook.h —— 成绩簿核心接口
//
// 这是一个用于「程序设计实践·工作流」章节演示的小型 CLI 项目：
// 读取 CSV 成绩单，输出每名学生的统计信息。
#pragma once

#include <string>
#include <vector>

namespace gradebook {

// 一名学生及其各次成绩
struct Student {
    std::string name;
    std::vector<int> scores;
};

// 解析 CSV 文本，每行形如 "姓名,分数1,分数2,..."。
// 空行会被跳过；以 "name" 开头的表头行也会被跳过。
std::vector<Student> parseCSV(const std::string& text);

// 平均分。成绩为空时返回 0.0（调用方无需再做判空）。
double average(const std::vector<int>& scores);

// 最高分。成绩为空时返回 0。
int maxScore(const std::vector<int>& scores);

// 中位数。成绩为空时返回 0.0。
double median(const std::vector<int>& scores);

// 及格率：达到及格线（默认 60 分）的成绩占比，取值在 [0,1] 之间。
double passRate(const std::vector<int>& scores, int passMark = 60);

// 等级评定，分段规则（含下界）：
//   >= 90 为 'A'，>= 80 为 'B'，>= 70 为 'C'，>= 60 为 'D'，其余为 'F'。
char letterGrade(double score);

// 按某名学生的平均分给出等级。
char gradeOf(const Student& student);

// 生成文本报告，每行一名学生，形如：
//   Zhang San: average=86.67, max=95
// withStats 为 true 时追加 median 与 passRate；
// withGrade 为 true 时追加 grade。
std::string formatReport(const std::vector<Student>& students, bool withStats = false, bool withGrade = false);

}  // namespace gradebook
