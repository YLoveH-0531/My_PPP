# DOS 版超级玛丽（Mario & Luigi）

Mike Wiering 1994 年用 Turbo Pascal 写的马里奥克隆，6 关，作者允许免费拷贝。
`MARIO.zip` 是 Internet Archive `Dos_Mario` 条目里的原包，未做任何修改。

## 在 Win10/Win11 上运行
1. 拉取仓库后进入 `dos-mario` 目录。
2. 双击 `play.bat`。第一次运行会自动解压 DOSBox-X 和游戏，然后直接开始。
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

## 文件
| 文件 | 来源 |
|---|---|
| `MARIO.zip` | https://archive.org/details/Dos_Mario |
| `dosbox-x-vsbuild-win64-2026.10.01-portable.zip` | https://github.com/joncampbell123/dosbox-x/releases/tag/dosbox-x-v2026.10.01 （GPL，源码在同一仓库） |
| `SHA256SUMS` | 校验用，`sha256sum -c SHA256SUMS` |
