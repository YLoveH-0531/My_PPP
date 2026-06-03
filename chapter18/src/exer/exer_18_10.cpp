/**
 * @file exer_18_10.cpp
 * @author KaKaRot
 * @brief  PPP exercise 18.10 + tests (palindrome over a char array of length n).
 *
 *         NOTE: the loops below currently never advance first/last, so any
 *         palindrome with n>=2 makes them loop FOREVER. The tests will HANG
 *         until you add `++first; --last;` inside each while loop.
 *
 *         Spec asserted by the tests:
 *           - empty and single-char inputs are palindromes (true)
 *           - both functions agree for inputs with n <= MAX
 *
 * @version 0.1
 * @date 2026-06-03
 *
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#define MAX 100

bool is_palindrome_report(const char s[], int n)
{
    if (n > MAX) {
        std::cout << "[!]The length is too long..." << '\n';
    }
    int first = 0;
    int last = n - 1;
    while (first < last) {
        if (s[first] != s[last]) { return false; }
        ++first; --last;
    }
    return true;
}

bool is_palindrome_any(const char s[], int n)
{
    int first = 0;
    int last = n - 1;
    while (first < last) {
        if (s[first] != s[last]) { return false; }
        ++first; --last;
    }
    return true;
}

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

static int slen(const char* s) {
    int n = 0;
    while (s[n] != '\0') ++n;
    return n;
}

// Run the same expectations against both implementations.
template <typename Fn>
static void run_cases(const char* name, Fn f) {
    std::cout << "-- " << name << " --\n";

    CHECK(f("racecar", slen("racecar")) == true);
    CHECK(f("abba",    slen("abba"))    == true);
    CHECK(f("noon",    slen("noon"))    == true);
    CHECK(f("aa",      2)               == true);
    CHECK(f("a",       1)               == true);
    CHECK(f("",        0)               == true);

    CHECK(f("hello",   slen("hello"))   == false);
    CHECK(f("ab",      slen("ab"))      == false);
    CHECK(f("abca",    slen("abca"))    == false);
    CHECK(f("palindrome", slen("palindrome")) == false);
}

int main()
{
    run_cases("is_palindrome_report(s, n)", is_palindrome_report);
    run_cases("is_palindrome_any(s, n)",    is_palindrome_any);

    std::cout << "\n==== " << g_pass << " passed, " << g_fail << " failed ====\n";
    return g_fail == 0 ? 0 : 1;
}
