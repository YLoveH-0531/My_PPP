/**
 * @file     calculator_KaKaLot_v2.cpp
 * @author   KaKaRot
 * @date     14/4/2026
 * @brief    write calculator program
 * @details  calculator improvement
 */

#include <exception>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "std_lib_facilities.h"

// constants
const char number = '8';       // signal that a number was found
const char quit = 'q';         // signal that we want to quit
const char print = ';';        // signal that we want to print the result
const char result = '=';       // signal that a result was found
const char prompt = '>';       // prompt for input
const char let = 'l';          // declaration token
const char name = 'a';         // name token
const char func = 'f';         // function token
const char cons = 'c';         // constant token
const std::string declkey = "let";  // declaration keyword

// function declaration
bool is_declared(string var);

// declaration keyword
// 将输入谓词化，不用处理char, 降低处理难度
class Token{
public:
    Token() : kind{0} {}
    Token(char ch) :kind{ch} { }
    Token(char ch, double val) :kind{ch}, value{val} { } // initialize kind
    Token(char ch, string n) :kind{ch}, name{n} { }
    char kind;
    double value;
    string name;
};

class Variable{
public:
    std::string name;
    double value;
    bool constant;
};

class Symbol_table
{
    public:
        void set_value(std::string s, double d);
        double get_value(const std::string& s);
        bool is_declared(string var);
        double define_name(string var, double val, const char kind);
    private:
        std::vector<Variable> var_table;
};

Symbol_table st;

void set_value(std::string s, double d)
{
    for (Variable& v : var_table)
        if (v.name == s) {
            if (v.constant)
            {
                error("set: constant variable");
            }
            v.value = d;
            return;
        }
    error("set: undefined variable ", s);
}

double get_value(const std::string& s)
{
    for (const Variable& v : var_table)
        if (v.name == s) return v.value;
    error("get: undefined variable ", s);
    return 0;
}

// get token and put back token.
class Token_Stream{
    public:
        Token_Stream(): full(false), buffer(0) { }
        void putback(Token t);
        Token get();
        void ignore(char c);
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
        case quit:    // q for quit
        case print:   // ; for output
        case '(': 
        case ')':
        case '{': 
        case '}':
        case '!':
        case '+':
        case '-': 
        case '*': 
        case '/': 
        case '%':
        case '=':
        case ',':
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
            return Token(number, val);
        }
        default:
            if (isalpha(ch)) {
                string s;
                s += ch;
                while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s+=ch;
                cin.putback(ch);
                if (s == declkey) return Token{let}; // declaration keyword
                if (s == "const") { return Token{cons}; } // constant keyword
                if (s == "sqrt") { return Token{func, s}; } // sqrt operator
                if (s == "pow") { return Token{func, s}; } // pow operator
                return Token{name,s};
            }
            error("Bad token");
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

void Token_Stream::ignore(char c)
{
    if (full && buffer.kind == c)
    {
        full = false;
        return;
    }
    full = false;

    char ch;
    while (std::cin >> ch)
    {
        if (ch == c) { return; }
    }
}

double expression();

long long factorial_cal(int n)
{
    if (n < 0) { error("The value of factorial is negative!"); } 
    if (n == 0 || n == 1) { return 1;}
    if (n > 20) { error("Factorial overflow: max supported is 20!"); } 
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

double func_pow()
{
    Token t = ts.get();
    switch (t.kind) {
        case '(':
        {
            double val = expression();
            t = ts.get();
            if (t.kind != ',') { error("',' missing!"); }
            double i = expression();
            t = ts.get();
            if (t.kind != ')') { error("')' missing!"); }
            return pow(val, i);
        }
        default:
            error("pow functional bad token!");    
    }
}

double func_sqrt()
{
    Token t = ts.get();
    switch (t.kind) {
        case '(':
        {
            double val = expression();
            if (val < 0) { error("sqrt number less then zero!"); }
            t = ts.get();
            if (t.kind != ')') { error("')' missing!"); }
            return sqrt(val);                
        }
        default:
            error("sqrt functional bad token!");    
    }
}

double functional(const std::string& fun)
{

    if (fun == "sqrt") {
        return func_sqrt();
    }else if (fun == "pow") {
        return func_pow();
    }else {
        error("function name wrong!");
    }
}

double Assignment(const std::string& var_name)
{
    if (!is_declared(var_name)) error("name expected in assignment");
    Token t = ts.get();
    if (t.kind != '=') {
        ts.putback(t);
        return get_value(var_name);
    }
    double d = expression();
    set_value(var_name, d);
    return d;
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
            return factorial(val);
        }
        case '{':
        {
            double val = expression();
            t = ts.get();
            if (t.kind != '}') { error("'}' missing!"); }
            return factorial(val);
        }
        case number:
        {             
            return factorial(t.value) ;
        }
        case '-':
        {             
            return -primary() ;
        }
        case '+':
        {             
            return primary() ;
        }
        case name:
        {             
		    return Assignment(t.name);
        }
        case func:
        {   
            return functional(t.name);
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
            case '%':
            {
                double val = primary();
                if (val == 0) { error("modulo can not be zero!"); }
                left = fmod(left, val);
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

void clean_up_mess()
{
    ts.ignore(print);
}

bool is_declared(string var)
{ // is var already in var_table?
    for (const Variable& v : var_table)
        if (v.name == var) return true;
    return false;
}

double define_name(string var, double val, const char kind)
{ // add {var,val} to var_table
    if (is_declared(var)) error(var," declared twice");
    bool is_const = (kind == cons);
    var_table.push_back(Variable{var,val, is_const});
    return val;
}

double declaration(const char kind)
{
    Token t = ts.get();
    if (t.kind != name) error ("name expected in declaration");
    string var_name = t.name;
    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", var_name);
    double d = expression();
    define_name(var_name,d,kind);
    return d;
}

double statement()
{
    Token t = ts.get();
    switch (t.kind) {
        case let:
        case cons:
            return declaration(t.kind);
        default:
            ts.putback(t);
        return expression();
    }
}

void calculate()
{
    std::cout << "================================================================\n";
    std::cout << "                  Welcome to our simple calculator              \n";
    std::cout << "      Please enter expressions using floating-point numbers     \n";
    std::cout << "      +,-,*,/ operators are supported, q to quit, ; to print    \n";
    std::cout << "================================================================\n";
    while (std::cin) {
        try
        {
            std::cout << prompt;
            Token t = ts.get();
            while (t.kind == print) t = ts.get();
            if (t.kind == quit) { break; }
            ts.putback(t);
			cout << result << statement() << endl;            
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            clean_up_mess();
        }   
    }
}

int main()
{
    try {

        // predefine names:
        define_name("pi",3.1415926535,true);
        define_name("e",2.7182818284,true);

        calculate();
        keep_window_open();
        return 0;
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
        keep_window_open("~~");
        return 0;
    } catch (...) {
        std::cout << "error happend!" << '\n';
        keep_window_open("~~");
        return 0;
    }
}

/*
    Simple calculator

    This program implements a basic expression calculator.
    Input from cin; output to cout.

    The grammar for input is:

    Calculation:
        Statement
        Calculation Statement
        Help
        Print
        Quit
    Statement:
        Declaration
        Expression
    Declaration:
        "let" Name "=" Expression
        "const" Name "=" Expression
    Help:
        "h"
    Print:
        ";"
    Quit:
        "q"
    Expression:
        Term
        Expression "+" Term
        Expression "-" Term
    Term:
        Primary
        Term "*" Primary
        Term "/" Primary
        Term "%" Primary
    Primary:
        Number
        Name
        Function
        "(" Expression ")"
        "+" Primary
        "-" Primary
    Number:
        floating-point-literal
    Function:
        Name "(" Expression ")"
        Name "(" Expression "," Expression ")"
    Name:
        identifier
        Assignment
    Assignment:
        Name "=" Expression
    Input comes from cin through the Token_stream called ts.
*/