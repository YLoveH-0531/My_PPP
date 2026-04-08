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
        Token_Stream(): full(false), t(0) { }
        void putback(Token t);
        Token get();
    private:
        bool full;
        Token t;
};

Token_Stream ts;

double expression();

double primary()
{
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
            case '(':
            {
                double val = expression();
                t = ts.get();
                if (t.kind != ')') { error("')' missing!"); }
                return val;
            }
            case '8':
            {
                return t.value;
            }
            default:
                error("primary bad token!");
        }
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
            }
            case '/':
            {
                t = ts.get();
                double val = t.value;
                if (val == 0) { error("divition can not be zero!"); }
                left /= term();
                t = ts.get();
            }
            default:
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
            }
            case '-':
            {
                left -= term();
                t = ts.get();
            }
            default:
                return left;
        }
    }
}

int main()
{
    try {
        double val;
        while (std::cin) {
            Token t = ts.get();
            if (t.kind == 'q') { break; }
            if (t.kind == ';') {
                std::cout << "=" << val << '\n';
            }else {
                ts.putback(t);
            }
            val = expression();
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    } catch (...) {
        std::cout << "error happend!" << '\n';
    }
    return 0;
}