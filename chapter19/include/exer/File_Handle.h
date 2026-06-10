#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

#include <string>
#include <fstream>
#include <stdexcept>

// File_Handle：RAII 封装文件——构造时打开，析构时自动关闭
// 题目要求：constructor takes a string argument (file name),
//           opens the file in constructor, closes it in destructor
class File_Handle
{
public:
    // 构造：打开文件，指定模式（默认读写）
    // 打开失败抛 runtime_error
    explicit File_Handle(const std::string& filename,
                         std::ios::openmode mode = std::ios::in | std::ios::out)
        : fs(filename, mode), name(filename)
    {
        if (!fs.is_open())
            throw std::runtime_error("Cannot open file: " + filename);
    }

    // fstream 不可拷贝 → File_Handle 也不可拷贝，显式标注意图
    File_Handle(const File_Handle&)            = delete;
    File_Handle& operator=(const File_Handle&) = delete;

    // 可以移动（转移文件所有权）
    File_Handle(File_Handle&&)            = default;
    File_Handle& operator=(File_Handle&&) = default;

    // 析构：自动关闭文件（RAII 核心）
    ~File_Handle()
    {
        if (fs.is_open()) fs.close();
    }

    // 暴露底层流，供外部读写
    std::fstream& stream() { return fs; }

    // 查询
    bool        is_open()  const { return fs.is_open(); }
    std::string filename() const { return name; }

private:
    std::fstream fs;
    std::string  name;
};

#endif // FILE_HANDLE_H
