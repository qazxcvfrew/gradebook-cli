# gradebook-cli

一个用于《程序设计实践》工作流章节演示的小型 C++17 命令行项目：读取 CSV 成绩单，输出每名学生的统计信息。

项目本身很小，重点在于演示 **单人工作流** 与 **团队（Pull Request）工作流** 中常见的 Git 操作。

## 构建与运行

```bash
# Linux / macOS / MSYS2
make            # 生成 build/gradebook-cli
make test       # 编译并运行单元测试

# Windows PowerShell
powershell -File build.ps1          # 构建（自动选择 g++ 或 MSVC）
powershell -File build.ps1 -Test    # 只构建并运行测试
```

运行：

```bash
./build/gradebook-cli data/scores.csv
```

输出：

```
lisi: average=90, max=92
wangwu: average=75, max=80
zhangsan: average=93, max=95
```

## 数据格式

`data/scores.csv`，第一行为表头，其后每行一名学生：

```csv
name,math,english
lisi,88,92
wangwu,70,80
```

## 目录结构

```
src/      核心实现与命令行入口
tests/    无第三方依赖的断言式单元测试
data/     示例成绩数据
```

## 协作约定

- 分支命名：`feature/<用户名>-<功能简述>`，例如 `feature/lisi-add-stats`
- 提交信息：遵循 `<type>: <描述>`，一个提交只做一件事
- 主分支 `main` 保持可构建、可测试；功能改动走分支 + Pull Request
- 提交前先 `git pull`，推送前用 `git status` / `git diff` 自查
