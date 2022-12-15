#include<algorithm>
#include<iostream>
#include<cstdint>
#include<sstream>
#include<cstdlib>
#include<variant>
#include<deque>
#include<vector>
#include <FileTokenizer.h>

inline std::string trim(std::string& str)
{
    str.erase(str.find_last_not_of(' ')+1);
    str.erase(0, str.find_first_not_of(' '));
    return str;
}

std::vector<std::string> tokenize_list(const std::string& string)
{
    std::string str = string;
    str = str.substr(1, str.size() - 2); //remove braces
    std::vector<std::string> tokens;
    size_t in_list = 0;
    std::stringstream current_token;
    str = trim(str);
    for(size_t i = 0; i < str.size(); i++)
    {
        if(str[i] == '[')
        {
            current_token.put(str[i]);
            in_list++;
        }
        else if(str[i] == ']')
        {
            current_token.put(str[i]);
            in_list--;
        }
        else if(str[i] == ',' && in_list == 0)
        {
            tokens.push_back(current_token.str());
            current_token.str("");
        }
        else
        {
            current_token.put(str[i]);
        }
    }
    if( current_token.str() != "")
    {
        tokens.push_back(current_token.str());
    }
    return tokens;
}

bool is_list(const std::string& str)
{
    return str[0] == '[' && str[str.size()-1] == ']';
}
std::string to_list(const std::string& str)
{
    return "[" + str + "]";
}

int compare_pairs(const std::string& first, const std::string& second)
{
    if(is_list(first) && is_list(second))
    { 
        auto first_tokens = tokenize_list(first);
        auto second_tokens = tokenize_list(second);
        size_t i = 0;
        while(i < first_tokens.size() && i < second_tokens.size())
        {
            int ret = compare_pairs(first_tokens[i], second_tokens[i]);
            if (ret == -1)
            {
                return -1;
            }
            if (ret == 1)
            {
                return 1;
            }
            i++;
        }
        if( i == first_tokens.size() && i < second_tokens.size())
        {
            return -1;
        }
        if( i < first_tokens.size() && i == second_tokens.size())
        {
            return 1;
        }
        return 0;
    }
    else if (is_list(first) && !is_list(second))
    {
        std::string second_list = to_list(second);
        return compare_pairs(first, second_list);
    }
    else if (!is_list(first) && is_list(second))
    {
        std::string first_list = to_list(first);
        return compare_pairs(first_list, second);
    }
    else
    {
        int first_int = std::stoi(first.c_str(),nullptr,10);
        int second_int = std::stoi(second.c_str(),nullptr,10);
        if(first_int < second_int)
        {
            return -1;
        }
        if(first_int == second_int)
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day13.txt", '\n');
    size_t total = 0;
    size_t index = 1;
    std::vector<std::string> packets;
    for(std::string first_str = tokenizer.next().value_or("");
         !first_str.empty();
         first_str = tokenizer.next().value_or(""))
    {
        std::string second_str = tokenizer.next().value_or("");
        packets.push_back(first_str);
        packets.push_back(second_str);
        if(compare_pairs(first_str, second_str) == -1)
        {
            total += index;
        }
        tokenizer.next();
        index++;
    }
    std::cout << "part1: " << total << std::endl;
    
    //part2
    size_t part_two = 1;
    packets.push_back("[[2]]");
    packets.push_back("[[6]]");
    std::sort(packets.begin(),packets.end(), [](const std::string& first,const std::string& second)
        {return compare_pairs(first,second) == -1;});
    for(size_t i = 0; i < packets.size(); i++)
    {
        if(packets[i] == "[[2]]" || packets[i] == "[[6]]")
        {
            part_two *= (i+1);
        }
    }
    std::cout << "part2: " << part_two << std::endl;
    return 1;
}