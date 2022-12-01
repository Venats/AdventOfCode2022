#include "FileTokenizer.h"

FileTokenizer::FileTokenizer(const std::string& file_name, const char delim) : m_file_stream(file_name),
                                                                            m_delim(delim)

{}

std::optional<std::string> FileTokenizer::next()
{
    std::string token = "";
    if(std::getline(m_file_stream, token, m_delim))
    {
        return std::optional<std::string>(token);
    }
    return std::nullopt;
}