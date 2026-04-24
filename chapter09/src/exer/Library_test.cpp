#include "Library.h"
#include "Book.h"
#include "Patron.h"
#include "Chrono.h"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    // --- 创建日期 ---
    Chrono::Date date1(2026, Chrono::Month::jan, 1);
    Chrono::Date date2(2026, Chrono::Month::feb, 15);
    Chrono::Date date3(2026, Chrono::Month::mar, 10);

    // --- 创建书籍，ISBN 格式：整数-整数-整数-单字符 ---
    Book book1("The C++ Programming Language", "Bjarne Stroustrup", "100-200-300-X", date1);
    Book book2("Clean Code",                  "Robert C. Martin",  "200-300-400-A", date2);
    Book book3("Design Patterns",             "Gang of Four",      "300-400-500-B", date3);

    // --- 创建读者 ---
    Patron patron1("Alice", "P001");   // 无欠款
    Patron patron2("Bob",   "P002");   // 无欠款

    // --- 创建图书馆并添加书籍和读者 ---
    Library lib;
    lib.add_book(book1);
    lib.add_book(book2);
    lib.add_book(book3);
    lib.add_patron(patron1);
    lib.add_patron(patron2);

    std::cout << "=== Test 1: 正常借书 ===\n";
    try {
        lib.check_out_book("100-200-300-X", "P001", date1);
        std::cout << "Alice 借出《The C++ Programming Language》成功\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }

    std::cout << "\n=== Test 2: 借已借出的书 ===\n";
    try {
        lib.check_out_book("100-200-300-X", "P002", date1);
        std::cout << "Bob 借出《The C++ Programming Language》成功\n";
    } catch (const std::exception& e) {
        std::cout << "预期错误: " << e.what() << "\n";
    }

    std::cout << "\n=== Test 3: 读者卡号不存在 ===\n";
    try {
        lib.check_out_book("200-300-400-A", "P999", date2);
    } catch (const std::exception& e) {
        std::cout << "预期错误: " << e.what() << "\n";
    }

    std::cout << "\n=== Test 4: 还书 ===\n";
    try {
        lib.check_in_book("100-200-300-X", "P001");
        std::cout << "Alice 还书成功\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }

    std::cout << "\n=== Test 5: 还书后重新借出 ===\n";
    try {
        lib.check_out_book("100-200-300-X", "P002", date1);
        std::cout << "Bob 借出《The C++ Programming Language》成功\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }

    std::cout << "\n=== Test 6: 查询有欠款的读者 ===\n";
    std::vector<std::string> debtors = lib.owned_fees();
    if (debtors.empty()) {
        std::cout << "无读者有欠款\n";
    } else {
        for (const auto& name : debtors) {
            std::cout << "有欠款: " << name << "\n";
        }
    }

    return 0;
}
