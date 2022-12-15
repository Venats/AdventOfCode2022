#include<iostream>
#include<functional>
#include<cstdint>
#include<cstdlib>
#include<utility>
#include<set>
#include<limits>
#include<vector>
#include <FileTokenizer.h>

inline std::string trim(std::string& str)
{
    str.erase(str.find_last_not_of(' ')+1);
    str.erase(0, str.find_first_not_of(' '));
    return str;
}

std::vector<std::string> tokenize_string(std::string&& str, const std::string& delimiter)
{
    size_t pos = 0;
    str = trim(str);
    std::vector<std::string> tokens;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    tokens.push_back(str);
    return tokens;
}




std::set<std::pair<int,int>> ROCKS;
int ROCK_BOTTOM = 0;
int BELOW_ROCK_BOTTOM = 0;
void add_rocks_from_points(std::vector<std::pair<int,int>>&& rock_points)
{
    for(size_t i = 0; i < rock_points.size() -1; i++)
    {
        auto dx = std::abs(rock_points[i].first - rock_points[i+1].first);
        auto start_x = std::min(rock_points[i].first , rock_points[i+1].first);
        for(int x = 0; x <= dx; x++)
        {
            ROCKS.emplace(std::make_pair(start_x + x, rock_points[i].second));
        }

        auto dy = std::abs(rock_points[i].second - rock_points[i+1].second);
        auto start_y = std::min(rock_points[i].second , rock_points[i+1].second);
        ROCK_BOTTOM = std::max(start_y, ROCK_BOTTOM);
        BELOW_ROCK_BOTTOM = ROCK_BOTTOM + 2;
        for(int y = 0; y <= dy; y++)
        {
            ROCKS.emplace(std::make_pair(rock_points[i].first, start_y + y));
        }
    }
}

std::vector<std::pair<int,int>> parse_line_to_points(std::string&& str)
{
    std::vector<std::pair<int,int>> pairs;
    auto tokens = tokenize_string(std::move(str), " -> ");
    for(const auto& token : tokens)
    {
        int first = 0;
        int second = 0;
        sscanf(token.c_str(), "%d,%d", &first ,&second);
        pairs.emplace_back(std::make_pair(first, second));
    }
    return pairs;
}

bool update_sand_and_rocks(std::set<std::pair<int,int>>& rocks, std::pair<int,int>& sand)
{
    bool ret = false;
    if(sand.second+1 == BELOW_ROCK_BOTTOM)
    {
        rocks.emplace(sand);
        sand = std::make_pair(500,0);
        ret = true;
    }
    else if(rocks.find(std::make_pair(sand.first, sand.second+1)) == rocks.cend())
    {
        sand.second++;
    }
    else if(rocks.find(std::make_pair(sand.first-1, sand.second+1)) == rocks.cend())
    {
        sand.first--;
        sand.second++;
    }
    else if(rocks.find(std::make_pair(sand.first+1, sand.second+1)) == rocks.cend())
    {
        sand.first++;
        sand.second++;
    }
    else
    {
        rocks.emplace(sand);
        sand = std::make_pair(500,0);
        ret = true;
    }
    return ret;
}

size_t drop_sand(std::set<std::pair<int,int>> rocks)
{
    size_t num_sand = 0;
    std::pair<int,int> sand = std::make_pair(500,0);
    while(sand.second < ROCK_BOTTOM)
    {
        if(update_sand_and_rocks(rocks, sand))
        {
            num_sand++;
        }
    }

    return num_sand;
}

size_t drop_sand_part_2(std::set<std::pair<int,int>> rocks)
{
    size_t num_sand = 0;
    std::pair<int,int> sand = std::make_pair(500,0);
    while(rocks.find(std::make_pair(500,0)) == rocks.cend())
    {
        size_t old_size = rocks.size();
        update_sand_and_rocks(rocks, sand);
        if(rocks.size() > old_size)
        {
            num_sand++;
        }
    }

    return num_sand;
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day14.txt", '\n');
    for(std::string pairs_str = tokenizer.next().value_or("");
         !pairs_str.empty();
         pairs_str = tokenizer.next().value_or(""))
    {
        add_rocks_from_points(
                parse_line_to_points(std::move(pairs_str))
        );
    }

    std::cout << "part1: " << drop_sand(ROCKS) << std::endl;
    std::cout << "part2: " << drop_sand_part_2(ROCKS) << std::endl;
    return 1;
}