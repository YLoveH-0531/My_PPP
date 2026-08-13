# Vim 学习计划

> 目标：把 Vim 练到"日常编辑够快"，练习场景是 My_PPP 刷题项目。主线是 C++ 转型学习，Vim 是辅助技能，严格时间盒。

## 环境

已完成，无需再动：

- VS Code 装 `vscodevim.vim` 插件，WSL Remote 方式打开项目，CMake Tools / clangd / cppdbg 不受影响
- `~/.vimrc`：`nnoremap <C-q> <C-v>`（Ctrl+V 被终端 / VS Code 占用做粘贴，Block 选中改用 Ctrl+Q）
- `~/.zshrc`：`stty -ixon`（释放 Ctrl+Q）
- VS Code `settings.json`：`"vim.vimrc.enable": true`、`"vim.vimrc.path": "~/.vimrc"`，终端与 VS Code 键位保持一致

验证：`.cpp` 文件里 Normal 模式按 `Ctrl+Q` → 状态栏出现 `-- VISUAL BLOCK --`。

## 四周节奏

每天 15–20 分钟，练习素材用 My_PPP 里真实在改的代码，不用合成练习题。对应命令清单见 [vim-cheatsheet.md](./vim-cheatsheet.md)。

| 周 | 内容 | 对应知识层 |
|:---:|---|:---:|
| 1 | 进出 Insert + 移动 + 基础编辑，把 `Esc` 练成肌肉记忆 | 第 1–3 层 |
| 2 | 搜索替换 + `.` 重复 + "动词 + 范围"语法（`ciw` / `di"` / `dap` / `cf;`） | 第 4 层 |
| 3 | Visual / `Ctrl-v` 列编辑 + Shell 交互 | 第 5–6 层 |
| 4+ | 宏、标记 —— 手感起来后自然会用 | 第 7–8 层 |

## 学习资源

一条主线，不并行读多本书 / 看多个系列：

1. `vimtutor` — 已完成
2. [CoolShell《简明 Vim 练级攻略》](https://coolshell.cn/articles/5426.html) — 中文免费，体系化补全，第 1–2 周同步看
3. [ThePrimeagen · vim-fundamentals](https://github.com/ThePrimeagen/vim-fundamentals) — 进阶，第 4 周后再看，覆盖宏 / 寄存器 / 工作流
4. *(可选)* 《Practical Vim》— 想读书可以加，按书自身章节顺序读，不与前面并行

## 收口原则

- 4 周到点，停止专门学习新命令，回归 C++ 主线
- 判断标准是"手速跟得上思路"，不是"背熟所有命令"
- 复杂跨文件重构直接用 VS Code 原生 `F2` / 全局替换，不硬憋 Vim 命令

## 验证方式

- 每周结束前，挑一个当天在改的真实文件，限时用当周学的手法完成一次实际编辑，顺畅完成即通过
- 第 3 周后，额外确认终端 vim 与 VS Code 里同一操作（如 `Ctrl+Q` 进 Block）表现一致
