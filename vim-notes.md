# Vim 学习笔记(VSCode + WSL,配合 My_PPP 练习使用)

## 0. 背景与结论

目标:刷 PPP 练习题的同时,顺手把 vim 键位练熟。

- 环境已有 CMake Tools + clangd + cppdbg 调试,全部正常工作,**不需要换成裸 vim/neovim**。
- 折腾成本对比:
  - 裸 neovim 配置(clangd 接 `compile_commands.json` + 调试对齐现有 cppdbg 体验):一次性 4-8 小时,外加 2-4 周注意力被"学vim命令"和"学C++语法"两头分薄。
  - VSCode Vim 插件:零配置成本,不影响现有 CMake Tools / 调试链路,可以直接在刷题过程中顺手练。
- **结论:先用 VSCode + Vim 插件练键位,不要现在就换成裸 vim/neovim。** 等 PPP 刷完或者中途明显不想看这本书了,再考虑整套换环境,避免打断正在进行的学习节奏。

## 1. 安装与打开项目

安装插件:
```
code --install-extension vscodevim.vim
```
或在扩展面板搜 `Vim`(作者 vscodevim)。

打开项目(WSL 环境下):
```
cd /home/wjy/personal/CPlusPlus/My_PPP
code .
```
会自动以 Remote-WSL 方式打开,CMake Tools / clangd / cppdbg 照常生效。

验证插件生效:打开任意 `.cpp` 文件,状态栏最左侧出现 `-- NORMAL --` 之类的模式提示。没出现就 `Ctrl+Shift+P` → `Reload Window`。

> 踩坑记录:命令行 `code --list-extensions` 显示已安装,但 VSCode UI 里显示未安装 —— 通过 UI 扩展面板重新安装一次后恢复正常。

不受 Vim 插件影响、可以照常用的 VSCode 原生功能:`Ctrl+S` 保存、`Ctrl+A/C/V` 全选复制粘贴、`F5` 调试、CMake Tools 侧边栏、终端面板输入。

**键位同步**:`Ctrl+V` 被 VS Code 保留做粘贴(如上),所以 Normal 模式下真正触发列块选中的键是 `Ctrl+Q`(终端 `~/.vimrc` 里 `nnoremap <C-q> <C-v>` 的映射)。在 VS Code `settings.json` 里加:
```json
"vim.vimrc.enable": true,
"vim.vimrc.path": "~/.vimrc"
```
这样终端和 VSCodeVim 的键位保持一致,不用两边分别配置。

## 2. 核心心智模型

Vim 是三种模式的切换,不是"加了快捷键的编辑器":

- **Normal 模式**(默认,`Esc` 回到这里):发命令,不打字
- **Insert 模式**(`i`/`a`/`o` 等进入):正常打字
- **Visual 模式**(`v`/`V`/`Ctrl-v` 进入):选中文本再操作

唯一纪律:改完字立刻 `Esc` 回 Normal 模式,效率来自"待在 Normal 模式里发号施令"。

## 3. 分层命令参考(按学习顺序排列,不要跳级)

### 第1层:进出 Insert 模式

| 命令 | 作用 |
|---|---|
| `i` | 光标前插入 |
| `a` | 光标后插入 |
| `I` | 跳到行首插入(非空白字符处) |
| `A` | 跳到行尾插入 |
| `o` | 下方新开一行并插入 |
| `O` | 上方新开一行并插入 |
| `Esc` | 回 Normal 模式 |

### 第2层:移动

| 命令 | 作用 |
|---|---|
| `h j k l` | 左下上右 |
| `w`/ `e` / `b` | 下一个单词词头/下一个单词词尾/上一个单词头 |
| `0` / `$` | 行首/行尾 |
| `gg` / `G` | 文件开头/结尾 |
| `:` | 跳到某行 |
| `Ctrl-e` / `Ctrl-y` | 向下/向上翻半屏 |
| `Ctrl-d` / `Ctrl-u` | 向下/向上翻半屏 |
| `Ctrl-i` / `Ctrl-o` | 用于跳转列表，跳进/跳回 | 触发大跳转的opeator
                                                  - gg / G（跳到文件头/尾）
                                                  - /,? 搜索后回车
                                                  - :123 跳到某行
                                                  - %（匹配括号跳转）
                                                  - gd（跳到定义）
                                                  - 打开新文件
                                                  - Ctrl+]（跳到 tag） |

数字前缀:`5j` 下移5行,`3w` 跳3个词——`[次数][命令]` 是通用语法。

### 第3层:基础编辑
[基础操作]
| 命令 | 作用 |
|---|---|
|  `x`  | 删光标处字符 |
|  `r`  | 替换光标处一个字符 |
|  `R`  | 替换光标处连续几个字符 |
|  `dd` | 删整行 |
|  `cc` | 删整行, 并且进入到insert模式|
|  `yy` | 复制整行 |
|  `p` / `P` | 粘贴到下方/上方 |
|  `J` | 合并下一行到当前行末尾 |
|  `u` / `Ctrl-r` | 撤销/重做 |
|  `.` | 重复上一个操作(改一处后其他类似处一个 `.` 搞定) |

[进阶操作]
语法:`[number] operator [number]  motion`
- 动词:`d`(删) / `c`(删完直接进 Insert) / `y`(复制)
- 范围:`w`(词) / `$`(到行尾) / `i"` `i(` `i{`(引号/括号内部) / `ap`(段落/函数体)

常用组合:
- `ciw` = 改掉当前单词(改变量名/类型)
- `di"` = 删掉双引号里的内容
- `dap` = 删掉整个函数体/段落
- `cf;` = 改到下一个 `;` 为止

不用死记组合,记住"动词+范围"拼装规则,遇到新场景现拼。

### 第4层:搜索与替换

| 命令 | 作用 |
|---|---|
| `/关键词` + `Enter` | 向下搜索,`n`/`N` 跳下一个/上一个 |
| `*` | 搜索光标下的单词 |
| `:%s/旧/新/g` | 整个文件替换 |
| `:%s/旧/新/gc` | 替换前逐个确认 |
| `:g/pattern/normal 命令` | 对所有匹配行批量执行命令 |
  To substitute new for the first old in a line type    :s/old/new
  To substitute new for all 'old's on a line type       :s/old/new/g
  To substitute phrases between two line #'s type       :#,#s/old/new/g
  To substitute all occurrences in the file type        :%s/old/new/g
  To ask for confirmation each time add 'c'             :%s/old/new/gc
| `/` | 搜索后回车 (正向匹配) |
| `?` | 搜索后回车 (逆向匹配)|
| `%` | 跳到匹配的 `(){}[]` | usage : Place the cursor on any (, [, or { in the line below marked, than type the  % character |
 eg: how to find some contexts that constist of a few words?
     1.press v enter visual mode
     2.select the contents you want to search and press y to copy
     3.press / to ready to search
     4.press Ctrl + r to tell vim ready copy some thing from register.
     5.press " to copy contents form default register(step 2 did)
     6.press enter to serach.

### 第5层:Visual 模式

| 命令 | 作用 |
|---|---|
| `v` | 字符级选中 |
| `V` | 行级选中 |
| `Ctrl-v`(此环境下已重映射为 `Ctrl-q`,见第1节) | 列(块)选中 |
| 选中后 `d`/`y`/`c` | 删/复制/改选中内容 |

`Ctrl-v`/`Ctrl-q` 场景:多行变量声明前批量加 `static`,或多行行尾批量加 `;`——选中列,`I` 插入/`A` 追加,`Esc` 应用到所有行。

**陷阱**:退出 Insert 必须按真正的 `Esc`,按 `Ctrl+C` 只会改第一行,不会广播到其他选中行。

`v%` 组合:光标停在 `{` 上输入 `v%`,选中从这个 `{` 到匹配 `}` 之间整块内容,后接 `d`/`y`/`>` 操作整块代码。

### 第6层:在vim中使用 shell command.
|  命令  |  作用  |
|-------|-------|
| `:!command` | 执行shell command |
   !ls            shows a directory listing
   !rm filename   removes file FILENAME
| `:w filename`  | writes the current Vim file to disk with name FILENAME |
| `:r filename`  | retrieves disk file FILENAME and puts it below the cursor position |
| `:r !command`  | 回放5次reads the output of the dir command and puts it below the cursor position |


### 第7层:宏(重复性操作自动化):

| 命令 | 作用 |
|---|---|
| `qa` ... `q` | 录制到寄存器 a / 停止录制 |
| `@a` | 回放寄存器 a |
| `@@` | 重复上一次回放的宏 |
| `5@a` | 回放5次 |

典型场景:批量把多行测试数据从一种格式改成另一种格式,录一次后面用 `@@` 批量搞定。

**注意**:VSCodeVim 的宏/Ex 替换偶尔跟原生 vim 行为不完全一致,大批量重放前先用 2-3 行小范围验证。

### 第8层:标记与跳转历史

| 命令 | 作用 |
|---|---|
| `ma` | 在当前位置打标记 a |
| `` `a `` | 跳回标记 a |
| `Ctrl-o` / `Ctrl-i` | 跳转历史后退/前进 |

### 第9层:set command
:set nu -- display line number
:set nonu -- not display line number
:set hls -- highlight all matching phrases
:set nohls -- not highlight all matching phrases
:set ic -- ignore upper/lower case when searching 
:set noic -- not ignore upper/lower case when searching
:set is -- show partial matches for a search phrase 
:set nois -- not show partial matches for a search phrase 

## 4. `%` 命令详解(以 chapter20/src/exer/exer_20_2.cpp 为例)

```cpp
22  double* high(const double* first, const double* last)
23  {
24      double* result = nullptr;
...
26      for (int i = 0; i < last - first; i++)
27      {
28          if (result == nullptr || first[i] > *result)
29          {
30              result = const_cast<double*>(&first[i]);
31          }
32      }
33      return result;
34  }
```

**常见使用场景**
1. 校验函数/循环的作用域边界——光标放 `{` 上按 `%`,确认这对括号真正管辖到哪结束
2. 排查括号不匹配的报错——报错行往往不是真正漏括号的地方,用 `%` 定位预期外的跳转位置
3. 配合 Visual 模式整块选中(`v%`)——选中整个代码块再删/复制/缩进
4. **陷阱**:`%` 默认不认模板尖括号 `<>`(如 `std::vector<double>`),vim 没法区分 `<` 是括号还是小于号

**练习题**
1. 光标放第22行 `high(` 的 `(` 上按 `%` → 应跳到同行末尾 `)`
2. 光标放第23行 `{` 上按 `%` → 应跳到第34行 `}`(整个函数结束处)
3. 光标放第27行 `for` 的 `{` 上按 `%` → 应跳到第32行 `}`;光标放第29行 `if` 的 `{` 上按 `%` → 应跳到第31行 `}`(体会 `%` 精确分清嵌套层级)
4. 光标放第27行 `for` 的 `{` 上输入 `v%`,确认选中第27-32行整个循环体,按 `d` 删除再 `u` 撤销
5. 光标放第46行 `std::vector<double>` 的 `<` 上按 `%` → 确认**不会**跳到 `>`(验证 `<>` 陷阱)
6. 临时删掉第23行的 `{`,把光标放第36行 `Jack_and_Jill()` 的 `{` 上按 `%`,观察跳转位置是否异常,体会括号数量不对时的报警信号,**做完记得 `u` 撤销恢复**

## 5. 学习节奏建议

1. 第1周:第1-3层(insert 进出 + 移动 + 基础编辑),把 `Esc` 变成肌肉记忆
2. 第2周:第4层 `ciw`/`dap`/`di"` + `.` 重复
3. 第3周:第5层搜索替换 + 第6层 `Ctrl-v` 列编辑
4. 第4周及以后:宏和标记,等手感起来后自然会想用

## 6. 推荐学习资源

**国外(英文)**
- [Vim As Your Editor](https://www.youtube.com/playlist?list=PLm323Lc7iSW_wuxqmKx_xxNtJC_hJbQ7R) — ThePrimeagen,业内公认的 vim 布道者,讲解偏"为什么这么设计+实战工作流"
- [ThePrimeagen Vim Fundamentals](https://github.com/ThePrimeagen/vim-fundamentals) — 配套练习仓库,覆盖宏/寄存器/查找替换/vimrc配置,适合进阶
- [Vim Complete Tutorial for Beginners | 1 Hour Crash Course](https://www.youtube.com/watch?v=wq3Hk-ymFII) — 一小时体系过一遍,适合查漏补缺

**国内(中文)**
- [【保姆级入门】Vim编辑器 - bilibili](https://www.bilibili.com/video/BV13t4y1t7Wg/) — 技术蛋老师,B站最主流入门视频

**视频之外(性价比更高)**
- `vimtutor` — 装好 vim/neovim 后终端直接敲这个命令,自带约30分钟交互式练习,建议排在最前面做
- [简明 Vim 练级攻略 - CoolShell](https://coolshell.cn/articles/5426.html) — 陈皓写的经典中文教程,按"练级"分层讲解,体系化不啰嗦

**建议顺序**:`vimtutor`(30分钟打底)→ CoolShell 文章(体系补全)→ 有空再看 ThePrimeagen 进阶系列(宏/寄存器/工作流)。B站入门视频如果基础已会可以跳过。
