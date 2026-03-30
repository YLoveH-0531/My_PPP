/**
 * @file     exer_4_7.cpp
 * @author   KaKaRot
 * @date     2026
 * @brief    PPP Chapter 4 exercise 7.
 * @details  A more complicated caculater.
 */

 #include <iostream>
 #include <sstream>
 #include <vector>
 #include <string>

int phrase_number(const std::string& num)
{
   static const std::vector<std::string> vec_spelled{"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
   static const std::vector<std::string> vec_digit{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

   for (size_t i = 0; i < 10; i++)
   {
      if (num == vec_spelled[i] || num == vec_digit[i])
      {
         return i;
      }      
   }

   return -1;
}

int main()
{ 
   std::string line_input;

   std::cout << "================================================\n";
   std::cout << "    Mini Calculator (Single-digit only)\n";
   std::cout << " Example: '2 + three', 'four * 5', 'q' to quit\n";
   std::cout << "================================================\n";


   while(true)
   {
      std::cout << ">" << std::flush;
      if(!(std::getline(std::cin, line_input))) { std::cout << "Illegal Format. Re-input!" << std::endl; break;}
      
      std::stringstream ss(line_input);
      std::string num1, num2;
      char operation;

      if (ss >> num1 && num1 == "q") { std::cout << "Good Luck! Bye~" << std::endl; break;}
      
      if (ss >> num2 >> operation)
      {
         int phrasednum1 = phrase_number(num1);
         int phrasednum2 = phrase_number(num2);

         if (phrasednum1 != -1 && phrasednum2 != -1)
         {
            switch(operation)
            {
                case '+': 
                    std::cout << "The add of " << num1 << " and " << num2 << " is " << phrasednum1 + phrasednum2 << "\n";
                    break;
                case '-': 
                    std::cout << "The subtract of " << num1 << " and " << num2 << " is " << phrasednum1 - phrasednum2 << "\n";
                    break;
                case '*': 
                    std::cout << "The multiply of " << num1 << " and " << num2 << " is " << phrasednum1 * phrasednum2 << "\n";
                    break;
                case '/':
                    if(phrasednum2 == 0)
                    {
                        std::cout << "Error! Devided by 0" << "\n";
                    }
                    else
                    {
                        std::cout << "The divide of " << num1 << " and " << num2 << " is " << phrasednum1 / phrasednum2 << "\n";
                    }
                    break;
                default:
                    std::cout << "Invalid Operator! " << "\n";
                    break;
            } 
         }
         else{
            std::cout << "Numbers or operations illegal. Re-input! \n";
         }
      }
      else{
         std::cout << "Illegal Format. Re-input!" << "\n";
      }   
   }
   return 0;
}