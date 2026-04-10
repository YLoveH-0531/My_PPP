/**
 * @file     exer_6_5_6.cpp
 * @author   KaKaRot
 * @date     10/4/2026
 * @brief    PPP Chapter 6 exercise 5 && 6.
 * @details  English grammar
 */

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

const std::vector<std::string> Conjunction{"and", "or", "but"};
const std::vector<std::string> Article{"The"};
const std::vector<std::string> Noun{"birds", "fish", "C++"};
const std::vector<std::string> Verb{"rules", "fly", "swim"};

bool is_word_in(const std::string& word, const std::vector<std::string>& collection)
{
   if (std::find(collection.cbegin(), collection.cend(), word) != collection.cend()){ return true; }
   return false;   
}

bool read_sentence(std::vector<std::string>& sentence)
{
   while (true)
   {
      std::string token;
      if (!(std::cin >> token)){ return true; }
      if (sentence.empty() && token == "q"){ return true; }
      if (token == ".") { break; }
      sentence.push_back(token);
      if (token.back() == '.'){
         break;
      }  
   }
   return false;
}

bool is_subject(std::size_t& index, const std::vector<std::string>& sen)
{
   if (sen.empty()){ return false; }
   
   if (index < sen.size() && is_word_in(sen[index], Article))
   {
      index++;
   }

   if (index < sen.size() && is_word_in(sen[index], Noun)){
      index++;
      return true;
   }
      
   return false;
}

bool is_sentence(std::size_t& index, const std::vector<std::string>& sen)
{
   if (sen.empty()){ return false; }

   if (!is_subject(index, sen)) { return false; }

   if (index < sen.size() && is_word_in(sen[index], Verb)) { 
      index++; 
   }else{
      return false;
   }
   if (index < sen.size()) { 
      if (is_word_in(sen[index], Conjunction))
      {              
         index++;
         return is_sentence(index, sen);
      }else{
         return false;
      }
   }
   return true;
}

int main()
{
   std::cout << "==================================================================\n";
   std::cout << "                   Welcome to our English Grammer                 \n";
   std::cout << "      Please enter sentences (end with ' .', enter 'q' to quit)   \n";   
   std::cout << "==================================================================\n";
   while (true)
   {
         std::vector<std::string> sen;
         bool quit = read_sentence(sen);

         if (quit) { break; }
         
         std::size_t index = 0;
         bool ok = is_sentence(index, sen);

         for (const auto& token : sen)
         {
            std::cout << token << " ";
         }

         if (!sen.empty() && sen[sen.size() -1].back() != '.')
         {
            std::cout << ".";
         }
         
         std::cout << " is " << (ok ? "OK!" : "Not OK!") << "\n";     
   }
   return 0;
}

/*Gramer：
Sentence:
   Article Noun Verb
   Sentence Conjunction Sentence 
Conjunction:
   "and"
   "or"
   "but"
Article:
   "The"
Noun:
   "birds"
   "fish"
   "C++"
Verb:
   "rules"
   "fly"
   "swim
first thinking: 1.没有优先级要求，只有先后关系，所有没必要用递归下降思想. while + if (switch) 即可
                  2.需建立一个库，判断某个word是否属于某个规则.
*/
 