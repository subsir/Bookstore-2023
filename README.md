# 书店管理系统

SJTU ACM 班 2023 级程序设计课程大作业

## 目录

* [简介](#简介)
* [作业说明](#作业说明)
  * [分数组成](#分数组成)
  * [时间表](#时间表)
* [项目说明](#项目说明)
  * [开发要求](#开发要求)
    * [需求分析文档](#需求分析文档)
    * [总体设计文档](#总体设计文档)
    * [开发文档](#开发文档)
    * [Git 版本管理](#git-版本管理)
    * [代码风格](#代码风格)
  * [Bonus](#bonus)
  * [扣分](#扣分)

## 简介

实现一个用 C++ 语言编写的书店管理系统，用于向店家和顾客提供图书检索与购买相关服务。

本项目目标如下：

- 培养学生工程开发的能力（全套的开发过程与程序的封装等）；
- 培养学生编写文档的能力（需求文档/设计文档/开发文档的编写）；
- 培养学生的代码管理能力（git 的使用），拥有良好的代码规范意识；
- 提升学生的编程学习与实践能力（块状链表的学习实现）；
- 培养学生对编写软件的兴趣（见 [bonus](#bonus)）。

### 关于本仓库

关于本仓库有任何问题欢迎联系助教，也可以直接 pull request；本仓库内容将维护至本作业结束，请保持关注题面更改，助教也会及时发布相关通知。

## 作业说明

### 分数组成

本作业满分为 100%，最终将以本作业占本课程成绩总分分数折算入课程成绩。

| 得分项      |     分值 | 说明                              |
| ----------- | -------: | --------------------------------- |
| 正确性      |      55% | 通过所有公开数据点                |
| 鲁棒性      |      10% | 通过所有非公开数据点              |
| 文档编写    |      15% | 助教评分                          |
| Code Review |      20% | Git 仓库管理、代码规范等          |
| Bonus       | 最高 10% | 具体各项得分见 [bonus](#bonus)    |
| 总计        |     110% | 未通过正确性测试者 bonus 不能得分 |

### 时间表

助教将在固定时间节点检查学生的完成进度。如遇困难请**提前**联系助教。

- **Week 0** *（对应校历 2023-2024 学年第一学期第 12 周）*
  - 周一（11 月 27 日）：发布项目和 [业务要求文档](业务要求.md)，开始完成**需求分析文档**和**总体设计文档**；
- **Week 1** *（对应校历 2023-2024学年第一学期第 13 周）*
  - 周二（12 月 5 日）：检查需求分析文档和总体设计文档（分数占比 5pt）；发布**标准要求**文档；

- **Week 2**
  - 周一（12 月 11 日 18:30 前）：**通过**前置作业评测；
  - 周二（12 月 12 日）：**中期检查**，检查开发文档编写情况以及主体部分构思（分数占比 5pt）；
- **Week 4** *（对应校历第 16 周）*
  - 周一（12 月 25 日 18:30 前）：**通过**测试点评测；
    - 非公开测试点和 Bonus 部分截止日期同主体逻辑，超时完成的部分不计分；
  - 周二（12 月 26 日晚）：进行代码检查（Code Review）；

**友情提醒：临近期末，请尽早开工。**

## 项目说明

### 开发要求

每位学生均需完成一份需求分析文档、总体设计文档和开发文档。文档 **请使用 Markdown** 书写。

#### 需求分析文档

根据给出的要求文档，你需要用图或文字描述**书店系统**各个子系统的业务流程和数据流图。

#### 总体设计文档

你的总体设计文档应当包括如下部分：

- 功能设计
  - 请概述各模块的功能和功能结构图；

- 用户交互设计
  - 请设计所有的命令格式和对应的输出；
- 数据库设计
  - 请指出你需要存储的数据以及存储方式；
- 类、结构体设计

#### 开发文档

开发文档内容**无硬性要求**，内容可以参考以下条目：

- 项目名称 / 文档作者
- 程序功能概述
- 主体逻辑说明
- 代码文件结构，即 `main` 函数与各个类的关系
- 各个类的接口及成员说明
- 文件存储说明
- 其他补充说明

如果最后提交代码不符合开发文档则会酌情扣分。合理的文档修订是允许的，但请写明修订记录。

#### Git 版本管理

你需要了解版本库、工作区、暂存区等 git 基础概念，并掌握 git 的常用指令。

你需要合理使用 commit 功能维护代码版本。具体来说：

- Commit 频率不可以太低（**例如，不可以整个项目只有三四个 commit**）；
  - 频繁的 commit 有助于细致地保存代码变更的历史。
- Commit message 内容可以清晰简要但不能缺失、不能是无意义内容。
  - 对于比较大的 commit，鼓励用多行的 commit message 具体描述干了什么；
  - 合理的 message 示例：
    - docs: clarify design reasoning and details
    - partial implementation of unrolled linked list
    - fix: 切换帐户之前检查权限
  - 不合理的 message 示例：
    - update
    - Add files via upload
    - Update README.md
    - bugfix *（具体修了什么？）*
    - 20231122 *（git 会自动存储 commit 时间，不用在 commit message 里说明。）*

你的 git 仓库需要整体文件结构清晰，了解并自主设置 `.gitignore` 等文件，保证编译产物（`./code`、`cmake-build-debug` 等）和下发的测试数据点不进入 git 仓库。Git repository 总大小（包括 `.git` 文件夹在内的 worktree）不得超过 5 MiB。

**违反者会在 code review 中被扣分。**

#### 代码风格

选择合适的代码风格，严格遵守选定风格的代码规范。

### Bonus

见 [bonus 文档](bonus.md)。

### 扣分

请保证自己项目结构的可读性，可以包括优化项目结构、适当的文件树指南等，晦涩难懂的项目可能会加大助教的工作量，也可能会影响你的成绩。

**如有出现任何抄袭现象按 0 分计。**
