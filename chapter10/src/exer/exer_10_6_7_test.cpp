
  /**
   * @file     exer_10_6_7_test.cpp
   * @brief    Test cases for Roman_int class
   */

  #include "exer_10_6_7.h"
  #include <cassert>
  #include <sstream>
  #include <iostream>

  // ── 辅助宏 ────────────────────────────────────────────────
  #define TEST(name) void test_##name()
  #define RUN(name)  do { test_##name(); std::cout << "[PASS] " #name "\n"; } while(0)

  // ── 1. 基本转换：字符串构造 ──────────────────────────────
  TEST(basic_values) {
      assert(Roman_int("I").as_int()    == 1);
      assert(Roman_int("V").as_int()    == 5);
      assert(Roman_int("X").as_int()    == 10);
      assert(Roman_int("L").as_int()    == 50);
      assert(Roman_int("C").as_int()    == 100);
      assert(Roman_int("D").as_int()    == 500);
      assert(Roman_int("M").as_int()    == 1000);
  }

  // ── 2. 减法规则（subtractive notation）──────────────────
  TEST(subtractive_notation) {
      assert(Roman_int("IV").as_int()   == 4);
      assert(Roman_int("IX").as_int()   == 9);
      assert(Roman_int("XL").as_int()   == 40);
      assert(Roman_int("XC").as_int()   == 90);
      assert(Roman_int("CD").as_int()   == 400);
      assert(Roman_int("CM").as_int()   == 900);
  }

  // ── 3. 复合数字 ──────────────────────────────────────────
  TEST(composite_values) {
      assert(Roman_int("XIV").as_int()   == 14);
      assert(Roman_int("XLII").as_int()  == 42);
      assert(Roman_int("XCIX").as_int()  == 99);
      assert(Roman_int("CDXLIV").as_int()== 444);
      assert(Roman_int("MCMXCIX").as_int()== 1999);
      assert(Roman_int("MMXXVI").as_int() == 2026);
  }

  // ── 4. int 构造 → to_string 往返一致 ────────────────────
  TEST(int_constructor) {
      assert(Roman_int(1).to_string()    == "I");
      assert(Roman_int(4).to_string()    == "IV");
      assert(Roman_int(9).to_string()    == "IX");
      assert(Roman_int(2026).to_string() == "MMXXVI");
      assert(Roman_int(3999).to_string() == "MMMCMXCIX");
  }

  // ── 5. 大小写不敏感 ──────────────────────────────────────
  TEST(case_insensitive) {
      assert(Roman_int("iv").as_int()   == 4);
      assert(Roman_int("xiv").as_int()  == 14);
      assert(Roman_int("mcmxcix").as_int() == 1999);
  }

  // ── 6. operator<< ────────────────────────────────────────
  TEST(output_operator) {
      std::ostringstream oss;
      oss << Roman_int("XIV");
      assert(oss.str() == "XIV");

      oss.str("");
      oss << Roman_int(42);
      assert(oss.str() == "XLII");
  }

  // ── 7. operator>> ────────────────────────────────────────
  TEST(input_operator) {
      Roman_int r;
      std::istringstream iss("XLII");
      iss >> r;
      assert(r.as_int() == 42);
      assert(r.to_string() == "XLII");
  }

  // ── 8. operator+ ─────────────────────────────────────────
  TEST(addition_operator) {
      Roman_int a("X");   // 10
      Roman_int b("V");   // 5
      Roman_int c = a + b;
      assert(c.as_int()    == 15);
      assert(c.to_string() == "XV");

      Roman_int d("XIV");  // 14
      Roman_int e("XXVIII"); // 28
      assert((d + e).as_int() == 42);
  }

  // ── 9. 隐式转换 int ──────────────────────────────────────
  TEST(implicit_conversion_int) {
      Roman_int r("XLII");
      int v = r;          // operator int()
      assert(v == 42);
  }

  // ── 10. 无效输入抛出异常 ─────────────────────────────────
  TEST(invalid_input) {
      bool threw = false;
      try {
          Roman_int r("IZX");  // 'Z' 不合法
      } catch (const std::out_of_range&) {
          threw = true;
      }
      assert(threw);
  }

  // ── main ─────────────────────────────────────────────────
  int main() {
      RUN(basic_values);
      RUN(subtractive_notation);
      RUN(composite_values);
      RUN(int_constructor);
      RUN(case_insensitive);
      RUN(output_operator);
      RUN(input_operator);
      RUN(addition_operator);
      RUN(implicit_conversion_int);
      RUN(invalid_input);

      std::cout << "\nAll tests passed.\n";
      return 0;
  }