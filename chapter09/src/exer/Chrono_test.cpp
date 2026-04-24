/**
 * @file     Chrono_test.cpp
 * @author   KaKaRot
 * @date     23/4/2026
 * @brief    Test program for Chrono library
 * @details  Test for Chrono library
 */

#include "Chrono.h"
#include <iostream>

// helper: print Day as string
static const char* day_name(Chrono::Day d)
{
    switch (d) {
        case Chrono::Day::sunday:    return "Sunday";
        case Chrono::Day::monday:    return "Monday";
        case Chrono::Day::tuesday:   return "Tuesday";
        case Chrono::Day::wednesday: return "Wednesday";
        case Chrono::Day::thursday:  return "Thursday";
        case Chrono::Day::friday:    return "Friday";
        case Chrono::Day::saturday:  return "Saturday";
    }
    return "Unknown";
}

int main()
{
    using Chrono::Date;
    using Chrono::Month;
    using Chrono::Day;

    try {
        // --- Test 1: add_day 普通进位 ---
        std::cout << "=== Test 1: add_day 普通进位 ===\n";
        Date d1(2026, Month::jan, 30);
        std::cout << "Before: " << d1 << "\n";
        d1.add_day(3);   // 1月30 + 3 = 2月2日
        std::cout << "After add_day(3): " << d1 << "\n";  // expect (2026,2,2)

        // --- Test 2: add_day 跨年 ---
        std::cout << "\n=== Test 2: add_day 跨年 ===\n";
        Date d2(2025, Month::dec, 30);
        std::cout << "Before: " << d2 << "\n";
        d2.add_day(3);   // 12月30 + 3 = 1月2日
        std::cout << "After add_day(3): " << d2 << "\n";  // expect (2026,1,2)

        // --- Test 3: add_day 闰年 2月 ---
        std::cout << "\n=== Test 3: add_day 闰年2月 ===\n";
        Date d3(2024, Month::feb, 28);   // 2024 是闰年
        std::cout << "Before: " << d3 << "\n";
        d3.add_day(1);   // 闰年 2月28 + 1 = 2月29
        std::cout << "After add_day(1): " << d3 << "\n";  // expect (2024,2,29)
        d3.add_day(1);   // 2月29 + 1 = 3月1
        std::cout << "After add_day(1): " << d3 << "\n";  // expect (2024,3,1)

        // --- Test 4: add_month 普通 ---
        std::cout << "\n=== Test 4: add_month 普通 ===\n";
        Date d4(2026, Month::jan, 15);
        std::cout << "Before: " << d4 << "\n";
        d4.add_month(2);   // 1月 + 2 = 3月
        std::cout << "After add_month(2): " << d4 << "\n";  // expect (2026,3,15)

        // --- Test 5: add_month 跨年 ---
        std::cout << "\n=== Test 5: add_month 跨年 ===\n";
        Date d5(2026, Month::nov, 15);
        std::cout << "Before: " << d5 << "\n";
        d5.add_month(3);   // 11月 + 3 = 2月
        std::cout << "After add_month(3): " << d5 << "\n";  // expect (2027,2,15)

        // --- Test 6: add_month 日期截断 ---
        std::cout << "\n=== Test 6: add_month 日期截断 ===\n";
        Date d6(2026, Month::jan, 31);
        std::cout << "Before: " << d6 << "\n";
        d6.add_month(1);   // 1月31 + 1月 = 2月28（2026非闰年）
        std::cout << "After add_month(1): " << d6 << "\n";  // expect (2026,2,28)

        // --- Test 7: day_of_week 已知日期验证 ---
        std::cout << "\n=== Test 7: day_of_week ===\n";
        Date d7(2026, Month::apr, 24);   // 2026-04-24 是周五
        std::cout << "2026-04-24 is: " << day_name(Chrono::day_of_week(d7)) << "\n";  // expect Friday

        Date d8(2000, Month::jan, 1);    // 2000-01-01 是周六
        std::cout << "2000-01-01 is: " << day_name(Chrono::day_of_week(d8)) << "\n";  // expect Saturday

        Date d9(2026, Month::jan, 1);    // 2026-01-01 是周四
        std::cout << "2026-01-01 is: " << day_name(Chrono::day_of_week(d9)) << "\n";  // expect Thursday

        // --- Test 8: next_Sunday ---
        std::cout << "\n=== Test 8: next_Sunday ===\n";
        Date d10(2026, Month::apr, 24);  // 周五
        Date ns = Chrono::next_Sunday(d10);
        std::cout << "next Sunday after 2026-04-24: " << ns << "\n";    // expect (2026,4,26)
        std::cout << "  is: " << day_name(Chrono::day_of_week(ns)) << "\n";

        // --- Test 9: next_weekday ---
        std::cout << "\n=== Test 9: next_weekday ===\n";
        Date d11(2026, Month::apr, 24);  // 周五，下一个工作日是周一
        Date nw = Chrono::next_weekday(d11);
        std::cout << "next weekday after 2026-04-24: " << nw << "\n";   // expect (2026,4,27) Monday
        std::cout << "  is: " << day_name(Chrono::day_of_week(nw)) << "\n";

        Date d12(2026, Month::apr, 25);  // 周六，下一个工作日是周一
        Date nw2 = Chrono::next_weekday(d12);
        std::cout << "next weekday after 2026-04-25: " << nw2 << "\n";  // expect (2026,4,27) Monday
        std::cout << "  is: " << day_name(Chrono::day_of_week(nw2)) << "\n";
    }
    catch (Date::Invalid) {
        std::cerr << "Error: Invalid date\n";
        return 1;
    }
    return 0;
}
