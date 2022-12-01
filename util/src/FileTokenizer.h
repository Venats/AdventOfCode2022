#pragma once

#include <fstream>
#include <string>
#include <optional>

class FileTokenizer {
    public:
        FileTokenizer(const std::string& file_name, const char delim);
        ~FileTokenizer() = default;
        FileTokenizer(const FileTokenizer& other) = delete;
        FileTokenizer& operator=(const FileTokenizer& other) = delete;
        FileTokenizer& operator=(FileTokenizer&& other) = delete;

        std::optional<std::string> next();
    private:
        std::ifstream m_file_stream;
        const char m_delim;
};