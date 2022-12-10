#include<algorithm>
#include<iostream>
#include<utility>
#include<set>
#include<cstdint>
#include<cstdlib>
#include<vector>
#include <FileTokenizer.h>

enum Direction : char
{
    UP = 'U',
    DOWN = 'D',
    LEFT = 'L',
    RIGHT = 'R',
};

struct Instruction
{
    Direction dir;
    size_t amount;
};

struct HeadTailPair
{
    std::pair<int,int> head;
    std::pair<int,int> tail;
    std::set<std::pair<int,int>> tail_visited;
    HeadTailPair() : head(std::make_pair(0,0)),
        tail(std::make_pair(0,0)),
        tail_visited()
    {
        tail_visited.emplace(tail);
    }
    void move_head(const Direction& dir)
    {
        switch(dir)
        {
        case(UP):
            head.second++;
        break;
    
        case(DOWN):
            head.second--;
        break;

        case(LEFT):
            head.first--;
        break;

        case(RIGHT):
            head.first++;
        break;
        default:
        break;
        }
    }
    void move_tail(const std::pair<int,int>& head_position)
    {
        update_tail(head_position);
        tail_visited.emplace(tail);
    }
    private:
    void update_tail(const std::pair<int,int>& head_position)
    {
        int delta_x = head_position.first - tail.first;
        int delta_y = head_position.second - tail.second;
        if(std::abs(delta_x) <= 1 && std::abs(delta_y) <= 1)
        {
            return;
        }
        else if(std::abs(delta_y) >= 2 && std::abs(delta_x) >= 2)
        {
            tail.second = head_position.second > tail.second ? head_position.second - 1 : head_position.second + 1;
            tail.first = head_position.first > tail.first ? head_position.first - 1 : head_position.first + 1;
        }
        else if(std::abs(delta_y) == 2)
        {
            tail.second = head_position.second > tail.second ? head_position.second - 1 : head_position.second + 1;
            tail.first = head_position.first;
        }
        else if(std::abs(delta_x) == 2)
        {
            tail.first = head_position.first > tail.first ? head_position.first - 1 : head_position.first + 1;
            tail.second = head_position.second;
        }
    }
};

struct Puzzle
{
    std::vector<HeadTailPair> knots;
    Puzzle(size_t num_knots) : knots()
    {
        for(size_t i = 0; i < num_knots; i++)
        {
            knots.emplace_back();
        }
    }

    void resolve_instruction(const std::vector<Instruction>& instructions)
    {
        for(const auto& inst : instructions)
        {
            for(size_t i = 0; i < inst.amount; i++)
            {
                knots[0].move_head(inst.dir);
                knots[0].move_tail(knots[0].head);
                for(size_t j = 1; j < knots.size(); j++)
                {
                    knots[j].head = knots[j-1].tail;
                    knots[j].move_tail(knots[j].head);
                }
            }
        }
    }

};

int main()
{
    auto tokenizer = FileTokenizer("../input/day9.txt", '\n');
    Puzzle puzzle = Puzzle(9);
    std::vector<Instruction> instructions;
    for(std::string pairs_str = tokenizer.next().value_or("");
         !pairs_str.empty();
         pairs_str = tokenizer.next().value_or(""))
    {
        Instruction inst;
        sscanf(pairs_str.c_str(),"%c %lu", reinterpret_cast<char*>(&inst.dir), &inst.amount);
        instructions.emplace_back(inst);
    }
    puzzle.resolve_instruction(instructions);
    std::cout << "part1: " << puzzle.knots[0].tail_visited.size() << std::endl;
    std::cout << "part2: " << puzzle.knots.back().tail_visited.size() << std::endl;

    return 1;
}