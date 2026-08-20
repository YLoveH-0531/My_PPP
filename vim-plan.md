# Vim 学习计划

> 目标：把 Vim 练到"日常编辑够快"，练习场景是 My_PPP 刷题项目。主线是 C++ 转型学习，Vim 是辅助技能，严格时间盒。

## 环境

已完成，无需再动：

- VS Code 装 `vscodevim.vim` 插件，WSL Remote 方式打开项目，CMake Tools / clangd / cppdbg 不受影响
- `~/.vimrc`：`nnoremap <C-q> <C-v>`（Ctrl+V 被终端 / VS Code 占用做粘贴，Block 选中改用 Ctrl+Q）
- `~/.zshrc`：`[[ -t 0 ]] && stty -ixon`（释放 Ctrl+Q；2026-08-14 加了 `[[ -t 0 ]]` 判断，避免非 TTY 场景——比如 VS Code 探测 shell、脚本化调用——报 `Inappropriate ioctl for device`）
- VS Code `settings.json`：`"vim.vimrc.enable": true`、`"vim.vimrc.path": "~/.vimrc"`，终端与 VS Code 键位保持一致；另外加了 `"vim.number": true`、`"vim.relativenumber": true`（混合行号，跟 vimrc 无关，是 settings.json 直接管的）

验证：`.cpp` 文件里 Normal 模式按 `Ctrl+Q` → 状态栏出现 `-- VISUAL BLOCK --`。

> 2026-08-14 二次核实：读了本机装的 `vscodevim.vim` 扩展源码（`out/extension.js`），确认 vimrc 加载器**只解析 remap 相关的正则**（`nnoremap`/`inoremap`/`unmap` 等）和 `source` 指令，没有任何代码路径解析 `set` 命令——跟 [vim-notes.md](./vim-notes.md) 第41行"验证缺口"那条判断完全吻合，而且是设计上如此，不是"偶尔报bug"。结论：以后任何 `set xxx` 类配置都不要往 `~/.vimrc` 里加，直接写 VS Code `settings.json` 对应的 `vim.xxx` 项（上面行号那条就是例子）。

## 六周节奏

每天 15–20 分钟，练习素材用 My_PPP 里真实在改的代码，不用合成练习题。按《Practical Vim》自身章节顺序推进，命令细节对应查 [vim-notes.md](./vim-notes.md)（两套分类不是一一对应，粗略参照即可）。

| 周 | Practical Vim 对应主题 | 练习重点（C++/TS 场景） | 粗略对应 vim-notes.md 层 |
|:---:|---|---|:---:|
| 1–2 | Modes（模式与操作方式）+ Motions（行内/行间导航） | `f;`/`t=`/`;` 行内查找；相对行号 `Nj`/`Nk`；`cw` + `.` 重复 | 第 1–3 层 |
| 3–4 | Visual Mode + 文本对象 | `ci(`/`di(` 改参数列表；`ci{`/`vi{` 重写函数体；`ci"`/`ci'` 改字符串/`#include` 路径；`ci<` 改模板/泛型（`std::vector<int>`、TS `Record<string, any>`）；`ci[` 改数组方括号 | 第 3、5 层 |
| 5–6 | Registers（复制粘贴/宏）+ Patterns（搜索/替换/global 命令） | `qa...q` 录宏 + `N@a` 重放；`:%s/old/new/g`；`:g/pattern/normal ...` | 第 4、7、8 层 |

### 补充：按《Practical Vim》原书章节/Tip 序号的精细对照表

> 两张表选一张跟，别同时追进度——这张只是配合实体书页码/Tip 序号查进度用。

#### 第1周（对应上表 1–2周）
- **章节**：Ch1 The Vim Way + Ch2-5（Normal/Insert/Visual/Command-Line Mode）
- **Tip**：1–36
- 全书地基，逐条手敲验证

#### 第2周（对应上表 1–2周）
- **章节**：Ch6-7（多文件管理）+ Ch8（Motions）
- **Tip**：37–55
- Ch6-7 只泛读——VS Code 的 `Ctrl+P`/标签页已覆盖
- Ch8 是全书 ROI 最高的一章（text object/`f`/`t`/`%`），多留时间

#### 第3周（对应上表 3–4周）
- **章节**：Ch9（Jumps）+ Ch10-11（Copy&Paste/Macros）
- **Tip**：56–72
- 宏拿真实重复性改动练手（批量加头文件守卫、snake_case→camelCase 字段转换），别只敲书里示例

#### 第4周（对应上表 3–4周/5–6周过渡）
- **章节**：Ch12-13（正则匹配/Search）
- **Tip**：73–87
- 括号捕获组 `\(...\)` 放心练
- **跳过 `\v`(very magic)、`\zs`/`\ze`**——已用扩展源码 + GitHub issue（[#3073](https://github.com/VSCodeVim/Vim/issues/3073)、[#3996](https://github.com/VSCodeVim/Vim/issues/3996)、[#4018](https://github.com/VSCodeVim/Vim/issues/4018)）确认 VSCodeVim 未实现/有已知 bug

#### 第5周（对应上表 5–6周）
- **章节**：Ch14-15（替换/Global）+ Ch16-20（工具链）
- **Tip**：88–123
- Ch14 认真练
- Ch15 简单 `:g/pat/d` 没问题；复杂 `:g+normal+宏` 组合先在草稿 buffer 小范围试（参考本文档"环境"一节 + vim-notes.md 第4层的既有提醒）
- Ch16-20（ctags/quickfix/grep/自动补全/拼写检查）只泛读，功能基本被 VS Code 的 LSP 跳转/Problems 面板/全局搜索取代

#### 第6周（收口）
- **章节**：Ch21 + 附录 A1（vimrc 定制）
- 附录 A1 只对 remap 有效，跟本文档"环境"一节 2026-08-14 的核实结论一致——`set` 类配置认准 settings.json

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
