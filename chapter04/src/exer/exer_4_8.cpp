/**
 * @file     exer_4_8.cpp
 * @author   KaKaRot
 * @date     24/3/2026
 * @brief    PPP Chapter 4 exercise 8.
 * @details  chess and rice issue
 */

#include <iostream>

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
            calculate(riceNum);
        }
        else if(std::cin.eof())
        {
            if(std::cin.fail())
            {
                std::cout << "fail also set too\n";
            }
            std::cout << "[System] Input stream terminated by user. Exiting...\n";
            break;
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
                std::cout << "Illegal format! Re-input.\n";
            }
        }  
    }
    return 0;
}

/* some thinking
*  1. 标准输入的流程
*     a. 终端获取用户数据，会把用户写入数据的ASCII临时存放
*     b. 行规范层，系统内核等待终端的行数据输入完 + Enter, 把数据保存到内核的缓冲区
*     c. 代码层，std::cin << x, 会向系统发起一个read()的系统调用，然后内核把数据给到代码中的cin 缓冲区。
*  2.Ctrl + D 发生了什么
*     a. 终端不会写入任何字符，直接向系统内核发送一个控制命令
*     b. 内核接收到后，会在对应的 stdin 文件描述符上挂起一个 EOF 标志
*     c. 程序调用 read() 返回了0（表示读取了0个字节且是因为 EOF），这个状态就是持久化的, 所以std::cin的eof是置位的,并且由于读取失败，fail也会置位。
*  3.输入数据超过最大范围会发生什么？（在本例子中，当输入10000000000000超过 unsigned long long时候）
*     a. 终端依旧保存数据，等待回车给系统内核
*     b. 系统内核等待read()调用，把数据给cin的缓冲区
*     c. cin得到数据后，尝试转换为unsigned long long，发现溢出，在 C++11 及之后的标准中，溢出时 cin 会把这个数字从系统内核缓冲区拿走，
*        但同时会把自己的状态标记为 failbit（失败状态），并给变量赋一个最大值（或最小值）
*
*/