/**
 * @file exer_18_8_test.cpp
 * @author KaKaRot
 * @brief  Tests for PPP exercise 18.8 (is_palindrome overloads).
 *
 *         Specification asserted by these tests:
 *           - is_palindrome(s)            : std::string
 *           - is_palindrome(s, n)         : array of n chars (length given)
 *           - is_palindrome(first, last)  : `last` points to the LAST char
 *                                           (inclusive range), per PPP.
 *           - empty and single-char inputs are palindromes (true).
 *
 *         NOTE: the char-based overloads currently `return strcmp(...)`, whose
 *         result is INVERTED (strcmp==0 means equal -> palindrome), so those
 *         cases will fail until fixed. The std::string overload should pass.
 *
 * @version 0.1
 * @date 2026-06-03
 *
 * @copyright Copyright (c) 2026
 */

#include "exer_18_8.h"

#include <iostream>
#include <string>

// ---- tiny self-contained test harness (non-aborting) -----------------------
static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond)                                                            \
    do {                                                                       \
        if (cond) {                                                            \
            ++g_pass;                                                          \
        } else {                                                               \
            ++g_fail;                                                          \
            std::cout << "[FAIL] line " << __LINE__ << ": " #cond "\n";        \
        }                                                                      \
    } while (0)

// length helper (independent of the code under test)
static int slen(const char* s) {
    int n = 0;
    while (s[n] != '\0') ++n;
    return n;
}

// ---------------------------------------------------------------------------
static void test_string() {
    std::cout << "-- is_palindrome(const std::string&) --\n";

    CHECK(is_palindrome(std::string("racecar")) == true);
    CHECK(is_palindrome(std::string("abba"))    == true);
    CHECK(is_palindrome(std::string("a"))       == true);
    CHECK(is_palindrome(std::string(""))        == true);
    CHECK(is_palindrome(std::string("hello"))   == false);
    CHECK(is_palindrome(std::string("ab"))      == false);
    CHECK(is_palindrome(std::string("abca"))    == false);
}

static void test_array_n() {
    std::cout << "-- is_palindrome(const char s[], int n) --\n";

    CHECK(is_palindrome("racecar", slen("racecar")) == true);
    CHECK(is_palindrome("abba",    slen("abba"))    == true);
    CHECK(is_palindrome("a",       slen("a"))       == true);
    CHECK(is_palindrome("",        0)               == true);
    CHECK(is_palindrome("hello",   slen("hello"))   == false);
    CHECK(is_palindrome("ab",      slen("ab"))      == false);
}

static void test_first_last() {
    std::cout << "-- is_palindrome(first, last)  [last = last char] --\n";

    auto pal = [](const char* s) {
        int n = slen(s);
        return is_palindrome(s, s + n - 1);   // last points to the last char
    };

    CHECK(pal("racecar") == true);
    CHECK(pal("abba")    == true);
    CHECK(pal("a")       == true);   // first == last
    CHECK(pal("hello")   == false);
    CHECK(pal("ab")      == false);
    CHECK(pal("noon")    == true);
}

int main() {
    test_string();
    test_array_n();
    test_first_last();

    std::cout << "\n==== " << g_pass << " passed, " << g_fail << " failed ====\n";
    return g_fail == 0 ? 0 : 1;
}
