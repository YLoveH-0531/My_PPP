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

> 验证缺口:`vim.vimrc.enable` 官方文档标注为 experimental,原话"only remaps are supported, and you may experience bugs"——好在我们用的正是 `nnoremap`,在官方支持范围内。但这个映射目前只在终端 vim 里用 `verbose nmap` 验证过,还没人在 VS Code 里实测过 `Ctrl+Q` 是否真的进 Block,第一次在 VS Code 里用之前自己测一次。

**VS Code 原生能力复用**:VSCodeVim 默认就把几个 `g` 前缀键映射到 IDE 能力,不用额外配置——`gd` 跳转到定义(走 clangd/tsserver),`gh` 悬浮文档提示(等价于鼠标悬停),看完按 `Esc` 关掉。

**跳到报错处(波浪线)**:`F8`/`Shift+F8` 跳到下一个/上一个错误或警告——这是 VS Code 原生功能键,Vim 的 Normal 模式只捕获可打印字符键,不会拦截功能键,所以不用退出 Vim 直接按。跳过去后光标停在波浪线上,默认不会自动弹出错误文字,还需要按 `gh` 弹出悬浮框看完整错误信息(光标停在诊断位置时,`gh` 的悬浮框会把错误消息也带出来,不只是类型提示),看完 `Esc` 关掉。想一次看项目里全部错误用 `Ctrl+Shift+M` 打开 Problems 面板。

## 2. 核心心智模型

vim 是三种模式的切换,不是"加了快捷键的编辑器":

- **Normal 模式**(默认,`Esc` 回到这里):发命令,不打字
- **Insert 模式**(`i`/`a`/`o` 等进入):正常打字
- **Visual 模式**(`v`/`V`/`Ctrl-v` 进入):选中文本再操作

唯一纪律:改完字立刻 `Esc` 回 Normal 模式,效率来自"待在 Normal 模式里发号施令"。

## 3. 分层命令参考(按学习顺序排列,不要跳级)
@@some rules
  rule one: The Ideal: One Keystroke to Move, One Keystroke to Execute
## Normal mode ##
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
| f{char} |	查找并跳到下一个字符 {char}	向右 (Forward)
| F{char} |	查找并跳到上一个字符 {char}	向左 (Backward)
| t{char} |	查找并跳到下一个字符 {char} 之前 	向右
| T{char} |	查找并跳到上一个字符 {char} 之后 	向左
| ; |	重复上 一次的 f、F |、t 或 T 查找	与上次相同
| ,	| 反向重复上 一次的 f、F |、t 或 T 查找	与上次相反
| `gg` / `G` | 文件开头/结尾 |
| `:` | 跳到某行 |
| `num|` | 跳到某列 |
| `^` | 跳第一个非空白字符 |
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
|  `s`  |The s command compounds two steps into one: it deletes the character under the cursor and then enters Insert mode |
|  `dd` | 删整行 |
|  `cc` | 删整行, 并且进入到insert模式|
|  `yy` | 复制整行 |
|  `p` / `P` | 粘贴到下方/上方 |
|  `J` | 合并下一行到当前行末尾 |
|  `u` / `Ctrl-r` | 撤销/重做 |
|  `~`  | 切换光标处字符大小写,光标右移(可加count,如`3~`) |
|  `.` | 重复上一个操作(改一处后其他类似处一个 `.` 搞定) |

[进阶操作]
语法:`[number] operator [number]  motion`
- 动词:`d`(删) / `c`(删完直接进 Insert) / `y`(复制) / `gu`(变小写) / `gU`(变大写) / `g~`(切换大小写) / `>`(右移缩进) / `<`(左移缩进) / `=`(自动缩进) / `!`(外部命令过滤,见第6层)
- 范围:`w`(词) / `$`(到行尾) / `i"` `i(` `i{`(引号/括号内部) / `ap`(段落/函数体)

常用组合:
- `ciw` = 改掉当前单词(改变量名/类型)
- `di"` = 删掉双引号里的内容
- `dap` = 删掉整个函数体/段落
- `cf;` = 改到下一个 `;` 为止
- `ci<` / `di<` = 改模板/泛型尖括号内容(如 `std::vector<int>`、TS 的 `Record<string, any>`)
- `ci[` / `di[` = 改数组/索引方括号内容
- `guiw` / `gUiw` / `g~iw` = 光标下单词变小写/大写/大小写互换
- `>>` / `<<` / `==` = 当前行右移/左移缩进 / 按文件类型规则自动重新缩进
- 可视模式选中后直接按 `u`/`U`/`~`/`>`/`<`/`=` 也能对选区生效,不用记 operator+motion

**陷阱**:`u`/`U`/`~` 在 Normal 模式不是变大小写——`u`=撤销、`U`=恢复当前行、`~`=光标处单字符翻转大小写(可加count,如`3~`)。真正"选中文字变大小写"有两条路:Visual 模式选中后直接按 `u`(小写)/`U`(大写)/`~`(互换);或 operator 版 `gu`/`gU`/`g~` + motion(如`guiw`),同 d/c/y 一套语法。

不用死记组合,记住"动词+范围"拼装规则,遇到新场景现拼。

### 第4层:搜索与替换

| 命令                     | 作用                             |
|--------------------------|----------------------------------|
| `/关键词` + `ENTER`      | 向下搜索,`N`/`N` 跳下一个/上一个 |
| `*`                      | 搜索光标下的单词                 |
| `:%S/旧/新/G`            | 整个文件替换                     |
| `:%S/旧/新/GC`           | 替换前逐个确认                   |
| `:G/PATTERN/NORMAL 命令` | 对所有匹配行批量执行命令         |

> VSCodeVim 官方文档承认 Ex 命令行(`:g`/`:normal` 等进阶用法)有"固有限制",遇到 `:g`+`normal` 组合行为异常,直接退回 VS Code 原生方案:`Ctrl+H` 正则替换,或 `Ctrl+Shift+L` 选中所有匹配项做多光标编辑。

  To substitute new for the first old in a line type    :s/old/new
  To substitute new for all 'old's on a line type       :s/old/new/g
  To substitute phrases between two line #'s type       :#,#s/old/new/g
  To substitute all occurrences in the file type        :%s/old/new/g
  To ask for confirmation each time add 'c'             :%s/old/new/gc
| `/` | 搜索后回车 (正向匹配) |
| `?` | 搜索后回车 (逆向匹配)|
  ** some egs,
   1. Search offset 专题:`/pattern/offset<CR>`(与 `?` 对称)

   触发方式:直接按 `/` 或 `?`,不是 `:` 开头的Ex命令,但底层同样会短暂进入 Command-line 模式接收输入。

   offset 写法:

   | 类型 | 写法 | 落点 | 动作类型(配合operator时) |
   |---|---|---|---|
   | 不写 / `s`或`b` | `/pat/s`、`/pat/s+n`、`/pat/s-n` | 匹配开头(±n) | exclusive(排他,落点字符不算入) |
   | `e` | `/pat/e`、`/pat/e+n`、`/pat/e-n` | 匹配末尾(±n) | inclusive(包含,落点字符算入) |
   | 纯数字/`+n`/`-n` | `/pat/+2` | 匹配所在行的上/下第n行 | linewise(整行处理) |

   核心记忆:`e`系≈`f`(inclusive),`s`/`b`系≈`t`(exclusive)——跟第2层已学的f/t是同一套包含/排他逻辑,只是从单字符扩展到一次搜索匹配。

   三种用法:
   1. 单独按`/`/`?`:纯跳转光标
   2. 当operator的motion:`d/pattern/e<CR>`
   3. 当Ex range地址(只支持行级offset):`:1,/pattern/+2d`

| `%` | 跳到匹配的 `(){}[]` | usage : Place the cursor on any (, [, or { in the line below marked, than type the  % character |
 eg: how to find some contexts that constist of a few words?
     1.press v enter visual mode
     2.select the contents you want to search and press y to copy
     3.press / to ready to search
     4.press Ctrl + r to tell vim ready copy some thing from register.
     5.press " to copy contents form default register(step 2 did)
     6.press enter to serach.

### 第5层:宏(重复性操作自动化):

| 命令         | 作用                      |
|--------------|---------------------------|
| `qa` ... `q` | 录制到寄存器 a / 停止录制 |
| `@a`         | 回放寄存器 a              |
| `@@`         | 重复上一次回放的宏        |
| `5@a`        | 回放5次                   |

典型场景:批量把多行测试数据从一种格式改成另一种格式,录一次后面用 `@@` 批量搞定。

**注意**:VSCodeVim 的宏/Ex 替换偶尔跟原生 vim 行为不完全一致,大批量重放前先用 2-3 行小范围验证。

### 第6层:标记与跳转历史

| 命令 | 作用 |
|---|---|
| `ma` | 在当前位置打标记 a |
| `` `a `` | 跳回标记 a |
| `Ctrl-o` / `Ctrl-i` | 跳转历史后退/前进 |

### 第7层:set command
:set nu -- display line number
:set nonu -- not display line number
:set hls -- highlight all matching phrases
:set nohls -- not highlight all matching phrases
:set ic -- ignore upper/lower case when searching 
:set noic -- not ignore upper/lower case when searching
:set is -- show partial matches for a search phrase 
:set nois -- not show partial matches for a search phrase 

## insert mode ##
1. Keystrokes                Effect
   ----------------------------------------------------------------------------------
   | <C-h>                   Delete back one character (backspace)                  |
   | <C-w>                   Delete back one word                                   |
   | <C-u>                   Delete back to start of line                           |
   ----------------------------------------------------------------------------------
   | <Esc>                   Switch to Normal mode                                  |
   | <C-[>                   Switch to Normal mode                                  |
   | <C-o>                   Switch to Insert Normal mode                           |
   ----------------------------------------------------------------------------------
   | <C-v>{123}              Insert character by decimal code                       |
   | <C-v>u{1234}            Insert character by hexadecimal code                   |
   | <C-v>{nondigit}         Insert nondigit literally                              |
   | <C-k>{char1}{char2}     Insert character represented by {char1}{char2} digraph |
   ----------------------------------------------------------------------------------
   | <C-r>{register}         Insert the contents of register {register}             |
   | <C-r><C-p>{register}    Insert the contents of register {register},            |
   |                         fixing the indentation to match the current line       |
   ----------------------------------------------------------------------------------

## visual mode ##
1. Keystrokes command. 
   Keystrokes                Effect
   ----------------------------------------------------------------------------------
   | v 		                 Enable character-wise Visual mode                      |
   | V 		                 Enable line-wise Visual mode                           |
   | v 		                 Enable character-wise Visual mode                      |
   | V 		                 Enable line-wise Visual mode                           |
   | <C-v>	                 Enable block-wise Visual mode                          | 
   | gv		                 Reselect the last visual selection                     |      
   | gv		                 Reselect the last visual selection                     |      
   | o                       Go to other end of highlighted text                    |      
   ----------------------------------------------------------------------------------

## command line mode ##
1. Keystrokes command. 
   Keystrokes               Effect
   ----------------------------------------------------------------------------------
   | :[range]delete [x]      Delete specified lines [into register x]               |
   ----------------------------------------------------------------------------------
   | :[range]yank   [x]      Yank specified lines [into register x]                 |
   ----------------------------------------------------------------------------------
   | :[line]put     [x]      Put the text from register x after the specified line  |
   ----------------------------------------------------------------------------------
   | :[range]copy {address}  Copy the specified lines to below the line specified   |
   |                         by {address}                                           |
   ----------------------------------------------------------------------------------
   | :[range]move {address}  move the specified lines to below the line specified   |
   |                         by {address}                                           |
   ----------------------------------------------------------------------------------
   | :[range]normal {commands} Execute Normal mode {commands} on each specified line|
   ----------------------------------------------------------------------------------
   | :[range]join            Join the specified lines                               |
   ----------------------------------------------------------------------------------
   | :[range]substitute/{pat-tern}/{string}/[flags]                                 |
   |                         Replace occurrences of {pattern} with {string} on each | 
   |                         specified line                                         |
   ----------------------------------------------------------------------------------
   | :[range]global/{pattern}/[cmd]                                                 |
   |                         Execute the Ex command [cmd] on all specified lines    |
   |                         where the {pattern} matches                            | 
   ----------------------------------------------------------------------------------
2. Range display
   Symbol                   Address
   ----------------------------------------------------------------------------------
   | 0                       Virtual line above first line of the file              |
   | 1                       First line of the file                                 |
   | .                       Line where the cursor is placed                        |
   | $                       Last line of the file                                  |
   | %                       The entire file (shorthand for :1,$)                   |
   | 'm                      Line containing mark m                                 |
   | '<                      Start of visual selection                              |
   | '>                      First line of the file                                 |
   ----------------------------------------------------------------------------------
3. Other related commands
   Keystrokes               Effect
   ----------------------------------------------------------------------------------
   | @:                      repeat the last Ex command                             |
   | :%s//<C-r><C-w>/g       gets the word under the cursor                         |
   ----------------------------------------------------------------------------------
4. Keystrokes command. 
   Keystrokes               Effect
   ----------------------------------------------------------------------------------
   | :!{cmd}                 Execute {cmd} with the shell                           |
   | :shell                  Start a shell (return to Vim by typing exit)           |
   | :read !{cmd}            Execute {cmd} in the shell and insert its standard     |
   |                         output below the cursor                                |
   | :[range]write !{cmd}    Execute {cmd} in the shell with [range] lines as       |
   |                         standard input                                         |
   | :[range]!{filter}       Filter the specified [range] through external          |
   |                         program {filter}                                       |
   ----------------------------------------------------------------------------------
   some egs:
   | `:!command` | 执行shell command |
      !ls            shows a directory listing
      !rm filename   removes file FILENAME
   | `:'<,'>!command` | 可视模式选中后过滤:选中的行丢给外部命令处理,再用输出替换回选区 |
      :'<,'>!sort       选中行排序
      :'<,'>!sort -r    选中行逆序排序
      :'<,'>!nl         选中行前加行号(nl = number lines)
   | `:w filename`  | writes the current Vim file to disk with name FILENAME |
   | `:r filename`  | retrieves disk file FILENAME and puts it below the cursor position |
   | `:r !command`  | the output of the dir command and puts it below the cursor position |

5. Some use rules
`Ctrl-v`/`Ctrl-q` 场景:多行变量声明前批量加 `static`,或多行行尾批量加 `;`——选中列,`I` 插入/`A` 追加,`Esc` 应用到所有行。

1. 经典场景速查
- `delete`: `:g/pattern/d a` 批量删除匹配行,顺手备份到寄存器 a
- `yank`: `:g/pattern/y A` **必须用大写A追加**收集散落各处的匹配行——小写a会被`:g`逐行执行时反复覆盖,最终只剩最后一条(今天踩过的坑)
- `put`: 配合上面yank收集完,`:$put a` 一次性倾倒到文件末尾;注意`:put`永远是linewise,即使原寄存器是charwise也会另起新行
- `copy`(`:t`): `:g/pattern/t$` 复制匹配行到文件末尾,原处保留;比yank+put省一步,但不能像寄存器一样跨多次`:g`调用累积
- `move`(`:m`): `:g/pattern/m$` 移动匹配行到文件末尾,原处删除,常用于"归类整理"(比如把所有TODO集中到文件底部)
- `normal`: `:g/pattern/normal A -- reviewed` 给每个匹配行末尾批量追加统一文本,等价于对每行手动敲一遍键
- `join`: 取消强制换行用`:.,+3join`(纯范围拼接);`:g/pattern/j`是另一种用法,只合并"含pattern的行+下一行"
- `substitute`: `:g/keyword/s/foo/bar/g` 只在含关键词的行内替换,避免误伤全文件;正式替换前先用`n`标志统计命中数,再用`gc`逐个确认
- `global`: 本质是"先扫描全文标记匹配行,再逐行执行cmd",不会因为删行漂移;反选版是 `:v`(等价`:g!`),`:v/pattern/d` = 只保留匹配的行
- 补充: range 除了写行号,还能直接用搜索模式当地址,比如 `:1,/pattern/d`(删到第一次匹配该pattern的那一行,含它);行级offset(`+n`/`-n`)也能接在后面,如 `:1,/pattern/+2d`

记忆规则(容易搞混的两点):
- `n`/`N` 重放的是"上一条完整搜索命令"(pattern+offset打包),所以offset看起来被"记住"了
- 手动重打新命令时:pattern留空=复用上次(vim明确设计的特性);**offset留空≠复用上次**,就是"没提供",按默认(≈s)处理——两者留空的含义不对称

踩坑提醒:
- `d{motion}` 删除的是"起点到落点之间的整段范围",不是只删落点处——如果pattern在文件别处也出现,搜索可能跳到很远,中间所有内容(不管是不是同一个逻辑分区)都会被扫进去删掉
- `:p` 是 `:print`(老命令,只显示),不是 `:put` 的缩写,粘贴要用 `:put`/`:pu`

## 4. `%` 命令详解(以 chapter20/src/exer/exer_20_2.cpp 为例)
double* high(const double* first, const double* last)
{
    double* result = nullptr;

    for (int i = 0; i < last - first; i++)
    {
        if (result == nullptr || first[i] > *result)
        {
            result = const_cast<double*>(&first[i]);
        }
    }
    return result;
}

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
