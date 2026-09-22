/**
 * @file Mail_test.cpp
 * @author KaKaRot
 * @brief  Test cases for Mail_file::sub_map (Mail_Program.cpp)
 * @version 0.1
 * @date 2026-09-22
 *
 * @copyright Copyright (c) 2026
 */

#include "Mail_Program.h"
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#define TEST(name) void test_##name()
#define RUN(name)  do { test_##name(); std::cout << "[PASS] " #name "\n"; } while(0)

namespace {

const std::string kDataFile = std::string(DATA_DIR) + "Mail_data.txt";

int count_range(const Mail_file& mail, const std::string& subject) {
    auto pp = mail.sub_map.equal_range(subject);
    return static_cast<int>(std::distance(pp.first, pp.second));
}

}  // namespace

TEST(subject_matches_exactly_one_message) {
    Mail_file mail(kDataFile);
    auto pp = mail.sub_map.equal_range("Saying Hello");
    assert(std::distance(pp.first, pp.second) == 1);
    bool has_body = false;
    for (const auto& line : *(pp.first->second))
        if (line == "This is a message just to say hello.") has_body = true;
    assert(has_body);
}

TEST(subject_with_colon_prefix_matches_exactly) {
    Mail_file mail(kDataFile);
    auto pp = mail.sub_map.equal_range("Re: Saying Hello");
    assert(std::distance(pp.first, pp.second) == 1);
    bool has_body = false;
    for (const auto& line : *(pp.first->second))
        if (line == "This is a reply to your reply.") has_body = true;
    assert(has_body);
}

TEST(messages_without_subject_are_grouped_under_empty_key) {
    // 修复前 key 在 for 循环体内声明，每次迭代都被重置为空字符串，
    // 导致所有邮件（不管有没有 Subject 行）全部落在 "" 下面。
    // 修复后应该只有真的没有 Subject 行的邮件才落在这里。
    Mail_file mail(kDataFile);
    assert(count_range(mail, "") == 3);
    assert(count_range(mail, "Saying Hello") == 1);
    assert(count_range(mail, "Re: Saying Hello") == 1);
}

TEST(unknown_subject_returns_empty_range) {
    Mail_file mail(kDataFile);
    auto pp = mail.sub_map.equal_range("does not exist");
    assert(pp.first == pp.second);
}

TEST(sub_map_key_matches_find_subject_exactly) {
    // 修复前 sub_map 里存的地址是循环内局部 Message 的地址（悬垂指针），
    // 且 key 的前缀长度跟 find_subject() 不一致（多一个空格）。
    // 修复后二者应该完全一致，且指针指向的内容必须能安全读出。
    Mail_file mail(kDataFile);
    for (const auto& entry : mail.sub_map)
        assert(entry.first == find_subject(entry.second));
}

int main() {
    RUN(subject_matches_exactly_one_message);
    RUN(subject_with_colon_prefix_matches_exactly);
    RUN(messages_without_subject_are_grouped_under_empty_key);
    RUN(unknown_subject_returns_empty_range);
    RUN(sub_map_key_matches_find_subject_exactly);

    std::cout << "\nAll tests passed.\n";
    return 0;
}
