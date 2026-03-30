/**
 * @file    bleep.cpp
 * @author  KaKaRot
 * @date    2026
 * @brief   PPP Chapter 4 Try This: Bleep out disliked words.
 * @details Reads lines of text from standard input and replaces 
 * specific "disliked" words with a substitute string.
 */
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

bool match_fun(const std::string& word)
{
    static const std::vector<std::string> dislike_words {"quit", "exit", "come"};
    return std::find(dislike_words.cbegin(), dislike_words.cend(), word) != dislike_words.cend();
    
    //// 优化 1：将匹配逻辑改为数据驱动
    //return (str == "quit" || str == "exit" || str == "come");
}

int main() 
{
    std::string stringline;
    const std::string bleep_word = "good";

    std::cout << "Please input oneline contents: \n";
    while (std::getline(std::cin, stringline)) 
    {
        std::stringstream streamline(stringline);
        std::string word;

        //流式处理，去掉 vector 缓存，极大地节省了内存开销
        while(streamline >> word)
        {
            if(match_fun(word))
            {
                std::cout << bleep_word << " ";
            }
            else {
                std::cout << word << " ";
            }
        }
        std::cout << std::endl;
        std::cout << "Please re-input oneline contents: " << std::endl; ;
    }
    return 0;
}