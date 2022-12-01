#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<optional>
#include<string>
#include <FileTokenizer.h>

int main()
{
    auto tokenizer = FileTokenizer("../input/day1.txt", '\n');
    uint64_t max_cal = 0;
    uint64_t cur_cal = 0;
    auto token = tokenizer.next();
    while(token != std::nullopt)
    {
        std::string cal_str = token.value_or("");
        if(cal_str == "\r")
        {
            max_cal = std::max(max_cal,cur_cal);
            cur_cal = 0;
        }
        else
        {
            cur_cal += std::strtoull(cal_str.c_str(),nullptr,10);
        }
        token = tokenizer.next();
    }
    std::cout << max_cal << std::endl;
    return 1;
}