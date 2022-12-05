#include<iostream>
#include<cstdint>
#include<cstdlib>
#include <FileTokenizer.h>

int main()
{
    auto tokenizer = FileTokenizer("../input/day5.txt", '\n');
    uint32_t total_overlap = 0;
    uint32_t some_overlap = 0;
    for(std::string pairs_str = tokenizer.next().value_or("");
         !pairs_str.empty();
         pairs_str = tokenizer.next().value_or(""))
    {

    }
    std::cout << "part1: " << total_overlap << std::endl;
    std::cout << "part2: " << some_overlap << std::endl;
    return 1;
}