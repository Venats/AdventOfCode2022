#include<array>
#include<algorithm>
#include<numeric>
#include<iterator>
#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<string>
#include<set>
#include<vector>
#include <FileTokenizer.h>

static constexpr const size_t COMP_SIZE = 56;
static constexpr const size_t NUM_ELVES_IN_GROUP = 3;

struct Rucksack
{
    std::array<bool, COMP_SIZE> compartment_one;
    std::array<bool, COMP_SIZE> compartment_two;
    std::set<int32_t> points_in_sack;
    int32_t duplicate_pts;
    size_t calc_char_index(char c) const
    {
        size_t offset = (c < 'a') ? 'A' - 26 : 'a';
        return c - offset;
    }
    Rucksack(const std::string& ruck_str) : compartment_one(), compartment_two()
    {
        std::fill(compartment_one.begin(), compartment_one.end(), false);
        std::fill(compartment_two.begin(), compartment_two.end(), false);
        for (size_t i = 0; i < ruck_str.size()/2; i++ )
        {
            size_t index = calc_char_index(ruck_str[i]);
            points_in_sack.emplace(index+1);
            compartment_one[index] = true;
        }
        for (size_t i = ruck_str.size()/2; i < ruck_str.size(); i++ )
        {
            size_t index = calc_char_index(ruck_str[i]);
            points_in_sack.emplace(index+1);
            compartment_two[index] = true;
            if(compartment_one[index])
            {
                duplicate_pts = index+1; //priority is 1 based
            }
        }
    }
};

uint32_t points_for_group(std::vector<Rucksack>&& group_of_sacks)
{
    std::set<uint32_t> intersection;
    std::set_intersection(group_of_sacks[0].points_in_sack.begin(),group_of_sacks[0].points_in_sack.end(),
            group_of_sacks[1].points_in_sack.begin(),group_of_sacks[1].points_in_sack.end(),
            std::inserter(intersection, intersection.begin()));

    for(size_t i = 2 ; i < NUM_ELVES_IN_GROUP; i++)
    {
        std::set<uint32_t> intermediate_set;
        std::set_intersection(group_of_sacks[i].points_in_sack.begin(),group_of_sacks[i].points_in_sack.end(),
            intersection.begin(),intersection.end(),
            std::inserter(intermediate_set, intermediate_set.begin()));
        intersection = std::move(intermediate_set);
    }

    return *intersection.begin();
}


int main()
{
    auto tokenizer = FileTokenizer("../input/day3.txt", '\n');
    uint64_t total = 0;
    uint64_t badge_total = 0;
    std::vector<Rucksack> rucksacks;
    std::vector<Rucksack> group_of_sacks;
    for(std::string ruck_str = tokenizer.next().value_or("");
         !ruck_str.empty();
         ruck_str = tokenizer.next().value_or(""))
    {
        Rucksack sack = Rucksack(ruck_str);
        rucksacks.emplace_back(sack);
        group_of_sacks.emplace_back(std::move(sack));
        if(group_of_sacks.size() == 3)
        {
            badge_total += points_for_group(std::move(group_of_sacks));
            group_of_sacks.clear();
        }
    }
    total = std::accumulate(rucksacks.cbegin(), rucksacks.cend(), 0, 
        [](int acc, const Rucksack& sack){return acc + sack.duplicate_pts;});
    std::cout << "part1: " << total  << std::endl;
    std::cout << "part2: "  << badge_total << std::endl;
    return 1;
}