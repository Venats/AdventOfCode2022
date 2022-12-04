#include<iostream>
#include<cstdint>
#include<cstdlib>
#include <FileTokenizer.h>

struct Pairs {
    std::pair<int,int> pair_1;
    std::pair<int,int> pair_2;

    Pairs(const std::string& pairs_str) : pair_1(), pair_2()
    {
        sscanf(pairs_str.c_str(), "%d-%d,%d-%d",
            &pair_1.first,
            &pair_1.second,
            &pair_2.first,
            &pair_2.second);
    }
    bool contained_in(const std::pair<int,int>& first, const std::pair<int,int>&second)
    {
        return first.first >= second.first && first.second <= second.second;
    }

    bool contained_some(const std::pair<int,int>& first, const std::pair<int,int>&second)
    {
        return contained_in(first, second) ||
            (first.first <= second.first && first.second >= second.first) ||
            (second.first <= first.first && second.second >= first.first);
    }

    bool pairs_have_total_overlap()
    {
        return contained_in(pair_1, pair_2) || contained_in(pair_2, pair_1);
    }
    bool pairs_have_some_overlap()
    {
        return contained_some(pair_1, pair_2);
    }
};

int main()
{
    auto tokenizer = FileTokenizer("../input/day4.txt", '\n');
    uint32_t total_overlap = 0;
    uint32_t some_overlap = 0;
    for(std::string pairs_str = tokenizer.next().value_or("");
         !pairs_str.empty();
         pairs_str = tokenizer.next().value_or(""))
    {
        Pairs pairs = Pairs(pairs_str);
        if(pairs.pairs_have_total_overlap())
        {
            total_overlap++;
        }
        if(pairs.pairs_have_some_overlap())
        {
            some_overlap++;
        }
    }
    std::cout << "part1: " << total_overlap << std::endl;
    std::cout << "part2: " << some_overlap << std::endl;
    return 1;
}