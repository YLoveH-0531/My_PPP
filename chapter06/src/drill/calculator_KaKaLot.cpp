/**
 * @file     calculator_KaKaLot.cpp
 * @author   KaKaRot
 * @date     8/4/2026
 * @brief    write calculator program
 * @details  calculator
 */

#include <chrono>
#include <exception>
#include <iostream>
#include "std_lib_facilities.h"

// 将输入谓词化，不用处理char, 降低处理难度
class Token{
public:
    Token(char ch) : kind(ch), value(0) { }
    Token(char ch, double val) : kind(ch), value(val) { }
    char kind;
    double value;
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
        case '{': case '}':
        case '!':
        case '+': case '-': case '*': case '/':
        {
            return Token(ch);
        }
        case '.':
        case '0': case '1': case '2': case '3': case '4': 
        case '5': case '6': case '7': case '8': case '9':
        {
            double val;
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

double expression();

long long factorial_cal(int n)
{
    if (n < 0) { error("The value of factorial is negative!"); }
    
    if (n == 0 || n == 1) { return 1;}
    
    long long result = 1;
    for (int i = 2; i <= n; i++){
       result *=i;
    }
     
    return result;
}

double factorial(double value)
{
    Token t = ts.get();
    long long result = static_cast<int>(value);
    bool isFac = false;

    while (true) {
        switch (t.kind) {
            case '!':
            {
                isFac = true;
                result = factorial_cal(result);
                t = ts.get();
                break;
            }
            default:
                ts.putback(t);
                return isFac ? result : value ;
        }
    }
}

double primary()
{
    Token t = ts.get();
    switch (t.kind) {
        case '(':
        {
            double val = expression();
            t = ts.get();
            if (t.kind != ')') { error("')' missing!"); }
            return val;
        }
        case '{':
        {
            double val = expression();
            t = ts.get();
            if (t.kind != '}') { error("'}' missing!"); }
            return val;
        }
        case '8':
        {             
            return factorial(t.value) ;
        }
        case '-':
        {             
            return -primary() ;
        }
        default:
            error("primary bad token!");
    }
}

double term()
{
    double left = primary();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
            case '*':
            {
                left *= primary();
                t = ts.get();
                break;
            }
            case '/':
            {
                double val = primary();
                if (val == 0) { error("divition can not be zero!"); }
                left /= val;
                t = ts.get();
                break;
            }
            default:
                ts.putback(t);
                return left;
        }
    }
}

double expression()
{
    double left = term();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
            case '+':
            {
                left += term();
                t = ts.get();
                break;
            }
            case '-':
            {
                left -= term();
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
    try {
        std::cout << "================================================================\n";
        std::cout << "                  Welcome to our simple calculator              \n";
        std::cout << "      Please enter expressions using floating-point numbers     \n";
        std::cout << "      +,-,*,/ operators are supported, q to quit, ; to print    \n";
        std::cout << "================================================================\n";
        double val = 0;
        while (std::cin) {
            Token t = ts.get();
            if (t.kind == 'q') { break; }
            if (t.kind == ';') {
                std::cout << "=" << val << '\n';
            }else {
                ts.putback(t);
                val = expression();
            }
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    } catch (...) {
        std::cout << "error happend!" << '\n';
    }
    return 0;
}