/**
 * @file     exer_4_8_9.cpp
 * @author   KaKaRot
 * @date     24/3/2026
 * @brief    PPP Chapter 4 exercise 8 && 9.
 * @details  chess and rice issue
 */

#include <iostream>
#include <limits>

void calculate(unsigned long long count)
{

    if(count == 0)
    {
        std::cout << "0 grid have 0 rice\n" << std::endl;
        return ;
    }

    unsigned long long total_rice = 0;
    unsigned long long rice_per_grid = 0;
    size_t grid = 0;

    while(total_rice < count && grid < 64)
    {
        rice_per_grid = 1ULL << grid;
        total_rice += rice_per_grid;
        grid++;
        std::cout << "The " << grid << " grid have " << rice_per_grid << " rice\n";
    }
    if(total_rice >= count)
    {
        std::cout << "The total rice are " << total_rice << " and occurs in " << grid << " grid\n";       
    }
    else
    {
        std::cout << "Even the entire 64 grid aren't enough for " << count << " rice!\n";
    }
}

int main()
{
    std::cout << "===========================================\n" ;
    std::cout << "              Chess and Rice               \n";
    std::cout << "       Enter rice number('q' to quit )     \n";
    std::cout << "===========================================\n" ;

    while (true)
    {
        std::cout << ">" << std::flush;
        unsigned long long riceNum = 0;
        if (std::cin >> riceNum)
        {
            //calculate(riceNum);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if(std::cin.eof())
        {
            std::cout << "[System] Input stream isjnull, waiting for next input...\n";
            std::cin.clear();
            std::cout << "IO state: " << std::cin.rdstate() << "\n";
        }
        else{
            std::string cmd;
            std::cin.clear();
            if(std::cin >> cmd && cmd == "q")
            {
                std::cout << "Good luck! Bye~ \n";
                break;
            }
            else{
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Illegal format! Re-input.\n";
            }
        }  
    }
    return 0;
}

/* some thinking
*  1. 标准输入的流程
*     a. 终端只负责获取用户数据，会把用户写入数据的ASCII临时存放，并把数据下发到行规范层
*     b. 行规范层，它是终端和程序之间的一个“中间件”。默认情况下，是规范模式（canonical mode）。在这个模式下，行规范层维护着一个内核缓冲区，并遵循两个硬性规则：
*          1)拦截并响应控制字符：它会死死盯着你敲下的每一个键。如果发现是普通字符（a、1），就放进缓冲区；如果发现是特殊绑定的控制字符（比如对应中断的 Ctrl+C，或者对应 EOF 的 Ctrl+D），它会立刻拦截，绝不把这个字符放进缓冲区，而是执行相应的内核动作。
*          2)何时唤醒程序：只要缓冲区里没有遇到特定的“提交信号”，调用了 read() 的 C++ 程序就会一直阻塞（挂起睡眠)
*     c. 代码层，std::cin >> x, 会向系统发起一个read()的系统调用，然后行规范层如果遇到特殊字符 \n（Enter），放进缓冲区，并通知内核唤醒阻塞的 read() 调用；
*        发现是 Ctrl+D，拦截它，并通知内核立刻强制唤醒阻塞的 read() 调用，把缓冲区里的东西原封不动地交出去内核把数据给到代码中的cin 缓冲区。
*        --note:  cin >> x
*            → 先检查 basic_filebuf 的内部缓冲区有没有数据
*                ├─ 有数据 → 直接从缓冲区里读，不发起 read() 系统调用
*                └─ 没数据 → 调用 read()，一次性尽量多读（最多 8KB），填满缓冲区，再取数据
*  2.Ctrl + D 发生了什么
*     a. Ctrl+D 让行规范层把当前缓冲区（哪怕是空的）立刻 flush 给等待 read()的进程
*     b. 如果缓冲区为空，read() 返回 0，应用层把这个 0 解释为 EOF
*     c. 内核侧：fd 仍然是 open 状态，没有任何变化
*        如果再次调用 read(STDIN_FILENO, buf, n)，内核会正常阻塞，等待下一次输入, 如果用户继续输入，仍然能正常返回数据
*     d. 调用过程,
*        应用代码 (cin >> riceNum)
*            ↓
*        C++ 标准库层: basic_ios / basic_istream
*            ↓
*        C++ 标准库层: basic_filebuf::underflow()   ← C++ 对象级别
*            ↓
*        C 标准库层:   fread() / 或直接 read()
*            ↓
*        系统调用:     read(fd, buf, n)              ← 内核边界
*            ↓
*        内核: tty line discipline → 终端输入缓冲区
*        basic_filebuf::underflow() 完全是 C++ 库层面的，是 libstdc++/libc++ 里一个普通的 C++ 虚函数
*        ------------------------
*        问题完全在 C++ 层
*        内核:          fd 还活着，愿意继续 block
*        basic_filebuf: 内部 EOF 标记已设 → 不发起新的 read() → 直接返回 eof
*        ios 层:        eofbit 被设置
*        cin.clear():   只清了 ios 层的 bit，basic_filebuf 的内部状态没变
*
*  3.输入数据超过最大范围会发生什么？（在本例子中，当输入10000000000000超过 unsigned long long时候）
*     a. 终端依旧保存数据，等待回车给系统内核
*     b. 系统内核等待read()调用，把数据给cin的缓冲区
*     c. cin得到数据后，尝试转换为unsigned long long，发现溢出，在 C++11 及之后的标准中，溢出时cin 会把这个数字从自己的输入缓冲区消耗掉，
*        但同时会把自己的状态标记为 failbit（失败状态），并给变量赋一个最大值（或最小值）
*
*/