/**
 * @file exer_18_5_6_7_test.cpp
 * @author KaKaRot
 * @brief  Tests for PPP exercises 18.5-18.7 (cat_dot overloads).
 *
 *         Specification asserted by these tests:
 *           - cat_dot(a, b)        -> a + "." + b
 *           - cat_dot(a, b, ch)    -> a +  ch  + b
 *           - the char* overloads return a NEWLY ALLOCATED, '\0'-terminated
 *             C-string (caller deletes[]), and nullptr if either input is null.
 *
 *         NOTE: the current char* overloads have bugs (s2p aliased to s1, and
 *         double-increment copy loops), so those cases will fail/crash until
 *         fixed. The std::string overloads should pass as-is.
 *
 * @version 0.1
 * @date 2026-06-03
 *
 * @copyright Copyright (c) 2026
 */

#include "exer_18_5_6_7.h"

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

// Helpers that do NOT depend on the code under test.
static int slen(const char* s) {
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
    return *a == *b;
}

// ---------------------------------------------------------------------------
static void test_string_dot() {
    std::cout << "-- cat_dot(string, string) --\n";

    CHECK(cat_dot(std::string("Niels"), std::string("Bohr")) == "Niels.Bohr");
    CHECK(cat_dot(std::string("a"), std::string("b")) == "a.b");
    CHECK(cat_dot(std::string(""), std::string("")) == ".");
    CHECK(cat_dot(std::string("left"), std::string("")) == "left.");
    CHECK(cat_dot(std::string(""), std::string("right")) == ".right");
}

static void test_string_sep() {
    std::cout << "-- cat_dot(string, string, ch) --\n";

    CHECK(cat_dot(std::string("Niels"), std::string("Bohr"), '+') == "Niels+Bohr");
    CHECK(cat_dot(std::string("a"), std::string("b"), '_') == "a_b");
    CHECK(cat_dot(std::string(""), std::string(""), '-') == "-");
    CHECK(cat_dot(std::string("x"), std::string("y"), '.') == "x.y");
}

static void test_cstr_dot() {
    std::cout << "-- cat_dot(const char*, const char*) --\n";

    char* a = cat_dot("Niels", "Bohr");
    CHECK(a != nullptr);
    CHECK(eq(a, "Niels.Bohr"));
    CHECK(slen(a) == 10);            // 5 + 1 + 4, properly terminated
    delete[] a;

    char* b = cat_dot("a", "b");
    CHECK(eq(b, "a.b"));
    delete[] b;

    char* e = cat_dot("", "");
    CHECK(eq(e, "."));
    CHECK(slen(e) == 1);
    delete[] e;

    char* l = cat_dot("left", "");
    CHECK(eq(l, "left."));
    delete[] l;

    char* r = cat_dot("", "right");
    CHECK(eq(r, ".right"));
    delete[] r;

    // null handling
    CHECK(cat_dot((const char*)nullptr, "x") == nullptr);
    CHECK(cat_dot("x", (const char*)nullptr) == nullptr);
}

static void test_cstr_sep() {
    std::cout << "-- cat_dot(const char*, const char*, ch) --\n";

    char* a = cat_dot("Niels", "Bohr", '+');
    CHECK(a != nullptr);
    CHECK(eq(a, "Niels+Bohr"));
    CHECK(slen(a) == 10);
    delete[] a;

    char* b = cat_dot("a", "b", '_');
    CHECK(eq(b, "a_b"));
    delete[] b;

    char* e = cat_dot("", "", '-');
    CHECK(eq(e, "-"));
    delete[] e;

    CHECK(cat_dot((const char*)nullptr, "x", '.') == nullptr);
    CHECK(cat_dot("x", (const char*)nullptr, '.') == nullptr);
}

int main() {
    test_string_dot();
    test_string_sep();
    test_cstr_dot();
    test_cstr_sep();

    std::cout << "\n==== " << g_pass << " passed, " << g_fail << " failed ====\n";
    return g_fail == 0 ? 0 : 1;
}
