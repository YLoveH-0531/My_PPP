/**
 * @file     exer_4_12.cpp
 * @author   KaKaRot
 * @date     26/3/2026
 * @brief    PPP Chapter 4 exercise 12.
 * @details  Judge if a number is prime
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

static const std::vector<int> prime_cons{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

void judgePrime(const std::string& input, std::vector<int>& prime_vec)
{
   std::stringstream ss(input);
   std::string token;
   while(ss >> token)
   {
      try
      {
         std::size_t pos;
         int number = std::stoi(token, &pos);
         if (pos == token.size() && (std::find(prime_cons.cbegin(), prime_cons.cend(), number) != prime_cons.end()))
         {
            if(std::find(prime_vec.cbegin(), prime_vec.cend(), number) == prime_vec.end())
            {
               prime_vec.emplace_back(number);
            }  
         }        
      }
      catch(const std::exception&)
      {
        continue;
      }
   }
}

int main()
{

   std::cout << "===========================================\n";
   std::cout << "                 Prime Game                \n";
   std::cout << "   Enter a number(0 ~100) ('q' to quit )   \n";
   std::cout << "===========================================\n";

   std::string line_input;
   while(true)
   {
      std::cout << ">" << std::flush;
      if (!std::getline(std::cin, line_input)){ break; }
      if(line_input == "q") { break; }
      std::vector<int> prime;
      judgePrime(line_input, prime);
      if (prime.empty())
      {
         std::cout << "No prime\n";
      }
      else{
         std::cout << "Prime number: ";
         for (const auto& i : prime)
         {
            std::cout << i << " ";
         }
         std::cout << '\n'; 
      }
      if (std::cin.eof()) { break; }    
   }
   std::cout << "Good Luck! Bye~\n";
   return 0;
}

/* some thinking
1. getline 是从 cin 的缓冲区读走数据放进 string，不是给 cin 送数据。cin 是数据的来源，string是数据的终点
2. 伪代码对比：
   getline,
     istream& getline(istream& is, string& str, char delim = '\n')
  {
      str.clear();                        // 先清空目标 string
      bool extracted = false;
      ios_base::iostate state = goodbit;

      auto* sb = is.rdbuf();              // 拿到 cin 持有的 basic_filebuf

      while (true)
      {
          int c = sb->sbumpc();
          //  ↑ 从 basic_filebuf 缓冲区取一个字符
          //    如果缓冲区有字符 → 直接返回
          //    如果缓冲区空    → 调用 underflow() → 调用 read() → 阻塞/返回

          if (c == EOF)                   // underflow() 返回了 eof（read()=0）
          {
              state |= eofbit;            // getline 把 eofbit 写进 state
              break;
          }

          extracted = true;

          if (c == delim)                 // 找到 '\n'，正常结束
              break;

          str += static_cast<char>(c);    // 字符逐个追加到 string
      }

      if (!extracted)
          state |= failbit;               // 一个字符都没拿到（空行+直接EOF）

      is.setstate(state);                 // 把 state 写入 cin 的状态位
      return is;
  }
   cin ,
     istream& operator>>(istream& is, int& x)
  {
      // 1. 跳过前导空白（' ' '\t' '\n' 都跳）
      while (true) {
          int c = sb->sgetc();
          if (!isspace(c)) break;
          sb->sbumpc();           // 消耗掉这个空白字符
      }

      // 2. 逐字符读取数字，直到遇到空白或 EOF
      string buf;
      while (true) {
          int c = sb->sbumpc();   // 同样走 basic_filebuf → underflow() → read()

          if (c == EOF) { state |= eofbit; break; }
          if (isspace(c)) {
              sb->sputbackc(c);   // 把空白字符"退回"缓冲区，留给下一次读
              break;
          }
          buf += c;
      }

      // 3. 把 buf 转换成目标类型
      x = stoi(buf);

      is.setstate(state);
      return is;
  }
  ┌─────────────┬─────────────────────────┬───────────────────────┐
  │             │        cin >> x         │   getline(cin, str)   │
  ├─────────────┼─────────────────────────┼───────────────────────┤
  │ 停止条件    │ 遇到空白字符（含 '\n'）    │ 遇到 '\n' 或 EOF      │
  ├─────────────┼─────────────────────────┼───────────────────────┤
  │ '\n' 的处理 │ 退回缓冲区，不消耗        │ 消耗掉，不进入 string  │
  ├─────────────┼─────────────────────────┼───────────────────────┤
  │ 前导空白    │ 跳过                     │ 保留                  │
  ├─────────────┼─────────────────────────┼───────────────────────┤
  │ EOF 处      │ 同样置 eofbit            │ 同样置 eofbit         │
  └─────────────┴─────────────────────────┴───────────────────────┘

*/