#include<array>
#include<numeric>
#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<optional>
#include<string>
#include <FileTokenizer.h>

static constexpr const size_t NUM_ELVES = 3;

void push_new_max_cal(std::array<uint64_t,NUM_ELVES>& cal_arr, uint64_t new_max)
{
    for(auto& cal : cal_arr)
    {
        if(new_max >= cal)
        {
            std::swap(new_max, cal);
        }
    }
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day1.txt", '\n');
    std::array<uint64_t,NUM_ELVES> top_cals = {0,0,0};
    uint64_t cur_cal = 0;
    auto token = tokenizer.next();
    while(token != std::nullopt)
    {
        std::string cal_str = token.value_or("");
        if(cal_str == "\r")
        {
            push_new_max_cal(top_cals, cur_cal);
            cur_cal = 0;
        }
        else
        {
            cur_cal += std::strtoull(cal_str.c_str(),nullptr,10);
        }
        token = tokenizer.next();
    }
    uint64_t total = std::accumulate(top_cals.cbegin(), top_cals.cend(), 0);
    std::cout << total << std::endl;
    return 1;
}