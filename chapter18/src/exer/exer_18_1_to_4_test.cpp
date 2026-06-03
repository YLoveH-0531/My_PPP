/**
 * @file exer_18_1_to_4_test.cpp
 * @author KaKaRot
 * @brief  Tests for PPP exercises 18.1-18.4 (strdup / findx / strcmp).
 *
 *         The assertions describe the *intended* (correct / standard) behavior,
 *         so they double as a specification:
 *           - strdup must return the START of a '\0'-terminated copy.
 *           - strcmp uses the STANDARD sign convention:
 *               <0 if s1<s2, 0 if equal, >0 if s1>s2.
 *           - the `max` overloads must never read past `max` chars
 *             (so they are safe on non-terminated "fake" C-strings).
 *
 *         NOTE: with the current implementation these will fail/crash until
 *         the strdup (return-pointer + missing terminator) and strcmp (sign)
 *         bugs are fixed.
 *
 * @version 0.1
 * @date 2026-06-03
 *
 * @copyright Copyright (c) 2026
 */

#include "exer_18_1_to_4.h"

#include <iostream>

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

// Helpers that do NOT depend on the functions under test.
static int slen(const char* s) {                 // local strlen
    int n = 0;
    while (s[n] != '\0') ++n;
    return n;
}
static bool eq(const char* a, const char* b) {   // null-terminated equality
    if (!a || !b) return a == b;
    while (*a && *b) {
        if (*a != *b) return false;
        ++a; ++b;
    }
    return *a == *b;                             // both must hit '\0' together
}
static bool eq_n(const char* a, const char* b, int n) {  // first n chars
    for (int i = 0; i < n; ++i)
        if (a[i] != b[i]) return false;
    return true;
}

// ---------------------------------------------------------------------------
static void test_strdup() {
    std::cout << "-- strdup(const char*) --\n";

    char* a = strdup("hello");
    CHECK(a != nullptr);
    CHECK(eq(a, "hello"));          // correct content
    CHECK(slen(a) == 5);            // properly '\0'-terminated
    CHECK(a[5] == '\0');
    delete[] a;

    char* e = strdup("");           // empty string -> just a terminator
    CHECK(e != nullptr);
    CHECK(slen(e) == 0);
    CHECK(e[0] == '\0');
    delete[] e;

    CHECK(strdup(nullptr) == nullptr);

    // deep copy: result must be independent of the source buffer
    char src[] = "world";
    char* c = strdup(src);
    src[0] = 'X';
    CHECK(eq(c, "world"));
    delete[] c;
}

static void test_strdup_max() {
    std::cout << "-- strdup(const char*, max) --\n";

    char* a = strdup("hello", 3);   // copy at most 3 chars
    CHECK(a != nullptr);
    CHECK(slen(a) == 3);
    CHECK(eq(a, "hel"));
    delete[] a;

    char* b = strdup("hi", 10);     // string shorter than max
    CHECK(eq(b, "hi"));
    CHECK(slen(b) == 2);
    delete[] b;

    // fake (non-terminated) C-string: max must keep us in bounds
    char fake[3] = {'a', 'b', 'c'};          // NO '\0'
    char* f = strdup(fake, 3);
    CHECK(f != nullptr);
    CHECK(slen(f) == 3);
    CHECK(eq_n(f, "abc", 3));
    CHECK(f[3] == '\0');                      // copy itself must be terminated
    delete[] f;
}

static void test_findx() {
    std::cout << "-- findx(s, x) --\n";

    const char* s = "hello world";
    CHECK(findx(s, "world") == s + 6);
    CHECK(findx(s, "hello") == s + 0);
    CHECK(findx(s, "o w")   == s + 4);
    CHECK(findx(s, "xyz")   == nullptr);

    const char* t = "abcabc";
    CHECK(findx(t, "bc")  == t + 1);          // first occurrence
    CHECK(findx(t, "abc") == t + 0);

    const char* u = "aaa";
    CHECK(findx(u, "aa") == u + 0);           // overlapping, first match

    CHECK(findx("ab", "abc") == nullptr);     // needle longer than remaining

    const char* h = "hello";
    CHECK(findx(h, "lo") == h + 3);           // match at the very end
    CHECK(findx(h, "")   == h);               // empty needle -> start
}

static void test_findx_max() {
    std::cout << "-- findx(s, x, max) --\n";

    // max = max chars to read from the haystack s (safe on fake strings)
    char fake[5] = {'h', 'e', 'l', 'l', 'o'};   // NO '\0'
    CHECK(findx(fake, "ell", 5) == fake + 1);   // match fully inside bound
    CHECK(findx(fake, "lo",  5) == fake + 3);   // ends exactly at bound
    CHECK(findx(fake, "lox", 5) == nullptr);    // no match; must NOT over-read
    CHECK(findx(fake, "xyz", 5) == nullptr);
}

static void test_strcmp() {
    std::cout << "-- strcmp(s1, s2)  [standard sign convention] --\n";

    CHECK(strcmp("abc", "abc") == 0);
    CHECK(strcmp("abc", "abd") <  0);   // s1 < s2
    CHECK(strcmp("abd", "abc") >  0);   // s1 > s2
    CHECK(strcmp("ab",  "abc") <  0);   // shorter prefix is "less"
    CHECK(strcmp("abc", "ab")  >  0);
    CHECK(strcmp("",    "")    == 0);
    CHECK(strcmp("",    "a")   <  0);
    CHECK(strcmp("a",   "")    >  0);
}

static void test_strcmp_max() {
    std::cout << "-- strcmp(s1, s2, max)  [standard sign convention] --\n";

    CHECK(strcmp("abc", "abd", 2) == 0);   // first 2 chars equal
    CHECK(strcmp("abc", "abd", 3) <  0);   // difference within max
    CHECK(strcmp("abc", "abc", 3) == 0);

    // fake (non-terminated) strings compared only up to max chars
    char a[3] = {'x', 'y', 'z'};
    char b[3] = {'x', 'y', 'q'};
    CHECK(strcmp(a, b, 2) == 0);           // 'x','y' equal -> no over-read
    CHECK(strcmp(a, b, 3) >  0);           // 'z' > 'q'
}

int main() {
    test_strdup();
    test_strdup_max();
    test_findx();
    test_findx_max();
    test_strcmp();
    test_strcmp_max();

    std::cout << "\n==== " << g_pass << " passed, " << g_fail << " failed ====\n";
    return g_fail == 0 ? 0 : 1;
}
