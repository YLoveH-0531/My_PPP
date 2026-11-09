# DOS 版超级玛丽（Mario & Luigi）

Mike Wiering 1994 年用 Turbo Pascal 写的马里奥克隆，6 关。作者允许免费拷贝，但有条件，见文末“许可”。
`MARIO.zip` 是 Internet Archive `Dos_Mario` 条目里的原包，未做任何修改。

## 回家怎么玩（Win10/Win11）

`MARIO.EXE` 是 1994 年的 DOS 程序，64 位 Windows 不能直接双击运行，要靠 DOS 模拟器。这里自带 DOSBox-X 便携版，`play.bat` 会替你启动，**不用另外装任何东西**。

**先注意分支：`dos-mario/` 只在 `integration_on_work` 分支上**，`integration_home` 和 `main` 上没有。用下面任意一种办法拿到它：

- 单独克隆一份（不碰你现有的分支；公开仓库，不用登录）：
  ```
  git clone -b integration_on_work https://github.com/YLoveH-0531/My_PPP.git My_PPP-play
  ```
- 已经有家里的克隆：
  ```
  git fetch origin
  git switch integration_on_work
  ```
  玩完想回原来的分支，再 `git switch <原来的分支名>`。

然后：
1. 进入 `dos-mario` 目录。
2. 双击 `play.bat`。第一次运行会自动解压 DOSBox-X 和游戏（解压到 `game/` 和 `dosbox-x/`，已在 `.gitignore` 里），然后直接开始。
3. 退出游戏后窗口自动关闭。

## 键位
| 操作 | 按键 |
|---|---|
| 菜单 | 方向键 + Enter，Esc 返回 |
| 走 / 跑 | 左右键 / Ctrl + 左右键 |
| 跳 | Alt |
| 开火 | 空格（上下键瞄准） |
| 进水管 | 下 |
| 退出 | Esc |

## 常见问题
- 游戏太快或太慢：改 `mario.conf` 里的 `cycles=8000`，或在运行中按 Ctrl+F11 / Ctrl+F12 降速/加速。
- 想全屏：Alt+Enter。
- 重新解压：删掉 `game/` 和 `dosbox-x/` 两个文件夹再运行 `play.bat`。
- 看不到 `dos-mario` 目录：当前不在 `integration_on_work` 分支，见上面“先注意分支”。

## 文件
| 文件 | 来源 |
|---|---|
| `MARIO.zip` | https://archive.org/details/Dos_Mario |
| `dosbox-x-vsbuild-win64-2026.10.01-portable.zip` | https://github.com/joncampbell123/dosbox-x/releases/tag/dosbox-x-v2026.10.01 （GPL，源码在同一仓库） |
| `SHA256SUMS` | 校验用，`sha256sum -c SHA256SUMS` |

## 许可
作者在 ZIP 里的 `Readme.txt` 写了发布条款，大意是：这是免费软件，可以拷贝，但**不能对游戏或任何附带文件做任何改动**；不要加自己的说明、注册表、启动器、“某某制作”之类的东西；**原 ZIP 保持原样**。所以：

- `MARIO.zip` 保持原样，不改、不重新打包。启动脚本 `play.bat` 和配置 `mario.conf` 放在 ZIP 外面。
- 要分发给别人，就连整个 ZIP 原样给，别把 `MARIO.EXE` 单独拆出来。自己玩不受这条限制。
