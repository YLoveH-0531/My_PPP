/**
 * @file     exer_7.cpp
 * @author   KaKaRot
 * @date     13/4/2026
 * @brief    PPP Chapter 6 exercise 7.
 * @details  Bit operation
 */

#include <iostream>
#include <exception>
#include "std_lib_facilities.h"

class Token{
public:
    Token(char ch) : kind(ch), value(0) { }
    Token(char ch, int val) : kind(ch), value(val) { }
    char kind;
    int value;
};

// get token and put back token.
class Token_Stream{
    public:
        Token_Stream(): full(false), buffer(0) { }
        void putback(Token t);
        Token get();
    private:
        bool full;
        Token buffer;
};

Token_Stream ts;

Token Token_Stream::get()
{
    if(full){
        full = false;
        return buffer;
    }

    char ch;
    std::cin >> ch;
    switch(ch)
    {   
        case 'q':   // q for quir
        case ';':   // ; for output
        case '(': case ')':
        case '-':
        case '!': case '~': case '&': case '^': case '|':
        {
            return Token(ch);
        }
        case '0': case '1': case '2': case '3': case '4': 
        case '5': case '6': case '7': case '8': case '9':
        {
            int val;
            std::cin.unget();
            std::cin >> val;
            return Token('8',val);
        }
        default:
            error("bad token");
    }
}

void Token_Stream::putback(Token t)
{
    if (full)
    {
        error("already have one token");
    }
    buffer = t;
    full = true;
}

unsigned Exp();

unsigned NOTExp()
{
   Token t = ts.get();
   switch (t.kind) {
      case '(':
      {
         unsigned val = Exp();
         t = ts.get();
         if (t.kind != ')') { error("')' missing!"); }
         return val;
      }
      case '8':
      {             
         return t.value;
      }
      case '~':
      {             
         return ~NOTExp() ;
      }
      case '!':
      {             
         return !NOTExp() ;
      }
      case '-':
      {             
         return -NOTExp() ;
      }
      default:
         error("primary bad token!");
   }
}

unsigned ANDExp()
{
   unsigned left = NOTExp();
   Token t = ts.get();

   while (true) {
      switch (t.kind) {
         case '&':
         {
               left &= NOTExp();
               t = ts.get();
               break;
         }
         default:
               ts.putback(t);
               return left;
      }
   }
}

unsigned XORExp()
{
   unsigned left = ANDExp();
   Token t = ts.get();

   while (true) {
      switch (t.kind) {
         case '^':
         {
               left ^= ANDExp();
               t = ts.get();
               break;
         }
         default:
               ts.putback(t);
               return left;
      }
   }
}

unsigned Exp()
{
   unsigned left = XORExp();
   Token t = ts.get();

   while (true) {
      switch (t.kind) {
         case '|':
         {
               left |= XORExp();
               t = ts.get();
               break;
         }
         default:
               ts.putback(t);
               return left;
      }
   }

}

int main()
{
   try{
      std::cout << "================================================================\n";
      std::cout << "                Welcome to our simple bit operation             \n";
      std::cout << "            Please enter expressions using int numbers          \n";
      std::cout << "     !,~,&,|,^ operators are supported, q to quit, ; to print   \n";
      std::cout << "================================================================\n";
      unsigned val = 0;
      while (std::cin) {
            Token t = ts.get();
            if (t.kind == 'q') { break; }
            if (t.kind == ';') {
               std::cout << "=" << val << '\n';
            }else {
               ts.putback(t);
               val = Exp();
            }
      }
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    } catch (...) {
        std::cout << "error happend!" << '\n';
    }
    return 0;
}

/*Gramer：
Exp:
   XORExp
   Exp | XORExp // or operator
XORExp:
   ANDExp
   XORExp ^ ANDExp // ^ operator
ANDExp:
   NOTExp
   ANDExp & NOTExp   // & operator
NOTExp:
   "(" Exp ")"  // grouping
   ~(NOTExp)    // ~ operator
   !(NOTExp)    // ! operator
   Int_Number
Int_Number:
   int-literal
*/
 