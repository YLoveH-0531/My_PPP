# Vim 学习计划

> 目标：把 Vim 练到"日常编辑够快"，练习场景是 My_PPP 刷题项目。主线是 C++ 转型学习，Vim 是辅助技能，严格时间盒。

## 环境

已完成，无需再动：

- VS Code 装 `vscodevim.vim` 插件，WSL Remote 方式打开项目，CMake Tools / clangd / cppdbg 不受影响
- `~/.vimrc`：`nnoremap <C-q> <C-v>`（Ctrl+V 被终端 / VS Code 占用做粘贴，Block 选中改用 Ctrl+Q）
- `~/.zshrc`：`stty -ixon`（释放 Ctrl+Q）
- VS Code `settings.json`：`"vim.vimrc.enable": true`、`"vim.vimrc.path": "~/.vimrc"`，终端与 VS Code 键位保持一致

验证：`.cpp` 文件里 Normal 模式按 `Ctrl+Q` → 状态栏出现 `-- VISUAL BLOCK --`。

## 六周节奏

每天 15–20 分钟，练习素材用 My_PPP 里真实在改的代码，不用合成练习题。按《Practical Vim》自身章节顺序推进，命令细节对应查 [vim-notes.md](./vim-notes.md)（两套分类不是一一对应，粗略参照即可）。

| 周 | Practical Vim 对应主题 | 练习重点（C++/TS 场景） | 粗略对应 vim-notes.md 层 |
|:---:|---|---|:---:|
| 1–2 | Modes（模式与操作方式）+ Motions（行内/行间导航） | `f;`/`t=`/`;` 行内查找；相对行号 `Nj`/`Nk`；`cw` + `.` 重复 | 第 1–3 层 |
| 3–4 | Visual Mode + 文本对象 | `ci(`/`di(` 改参数列表；`ci{`/`vi{` 重写函数体；`ci"`/`ci'` 改字符串/`#include` 路径；`ci<` 改模板/泛型（`std::vector<int>`、TS `Record<string, any>`）；`ci[` 改数组方括号 | 第 3、5 层 |
| 5–6 | Registers（复制粘贴/宏）+ Patterns（搜索/替换/global 命令） | `qa...q` 录宏 + `N@a` 重放；`:%s/old/new/g`；`:g/pattern/normal ...` | 第 4、7、8 层 |

## 学习资源怎么用

`vimtutor` 已完成打底。之后不是几本书/几个系列并行看，而是**一条主线顺序读，其余按需穿插查阅**：

- **主线**：《Practical Vim》(Drew Neil) —— 按书自身章节顺序通读，不跳章，后面章节假设你已经掌握前面的动作/文本对象基础。
- **穿插资源**（不打断主线，只按需查阅，不整篇通读）：
  - [vimgolf.com](https://www.vimgolf.com/)：从第1周起贯穿全程，建议隔天打一题，练"用语法而不是方向键"的手感，题目不必匹配当周主题。
  - [Vim Galore](https://github.com/mhinz/vim-galore)：只翻跟当周主题同名的小节做补充，比如练 Visual 模式那周就去翻它的 Visual Mode 小节。
  - 《Learning the vi and Vim Editors》第7版 (O'Reilly)：只在第5–6周（Patterns/替换那两周）翻它的 Ex 命令行、正则替换章节，其余时间不用碰。
- **补充**（可选，非必需）：[CoolShell《简明 Vim 练级攻略》](https://coolshell.cn/articles/5426.html) —— 中文对照理解用；[ThePrimeagen · vim-fundamentals](https://github.com/ThePrimeagen/vim-fundamentals) —— 第6周后想深入宏/寄存器/工作流再看。
- **跳过**：Steve Oualline《Vi Improved》，2001 年后未更新，已过时。

## 收口原则

- 6 周到点，停止专门学习新命令，回归 C++ 主线
- 判断标准是"手速跟得上思路"，不是"背熟所有命令"
- 复杂跨文件重构直接用 VS Code 原生 `F2` / 全局替换，不硬憋 Vim 命令

## 验证方式

- 每两周结束前，挑一个当天在改的真实文件，限时用当阶段学的手法完成一次实际编辑，顺畅完成即通过
- 第 3–4 周后，额外确认终端 vim 与 VS Code 里同一操作（如 `Ctrl+Q` 进 Block）表现一致
