#include<array>
#include<algorithm>
#include<numeric>
#include<iostream>
#include<optional>
#include<cstdint>
#include<cstdlib>
#include<vector>
#include <FileTokenizer.h>

std::vector<std::string> tokenize_string(std::string&& str, const std::string& delimiter)
{
    size_t pos = 0;
    std::vector<std::string> tokens;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    tokens.push_back(str);
    return tokens;
}

enum OpCode
{
    ADD,
    NOOP
};

static const std::array<size_t, 6> SPECIAL_CYCLES = {20,60,100,140,180,220};
static constexpr const size_t CRT_WIDTH = 40;
static constexpr const size_t CRT_HEIGHT = 6;

struct Instruction
{
    OpCode code;
    std::optional<int> value;
    std::vector<int> saved_values;
    Instruction(const std::vector<std::string>& str_tokens) : code(), value()
    {
        if(str_tokens[0] == "addx") 
        {
            code = OpCode::ADD;
            value = std::stoi(str_tokens[1].c_str(),NULL,10);
        }
        else
        {
            code = OpCode::NOOP;
            value = std::nullopt;
        }
    }
};

struct Circuit
{
    size_t cycle;
    int reg_x;
    std::vector<int> special_cycle_reg_values;
    std::array<std::array<char,CRT_WIDTH>, CRT_HEIGHT> crt;
    Circuit() : cycle(0), reg_x(1)
    {}
    bool is_sprite_visible(ssize_t crt_horizontal)
    {
        return  reg_x == crt_horizontal || 
                reg_x+1 == crt_horizontal ||
                reg_x-1 == crt_horizontal;
    }
    void draw_crt_pixel()
    {
        size_t horizontal = cycle % CRT_WIDTH;
        size_t vecticle = cycle / CRT_WIDTH;
        crt[vecticle][horizontal] = is_sprite_visible(horizontal) ? '#' : '.';
    }
    void increment_cycle()
    {
        draw_crt_pixel();
        cycle++;
        if(std::find(SPECIAL_CYCLES.cbegin(), SPECIAL_CYCLES.cend(), cycle) != SPECIAL_CYCLES.cend())
        {
            std::cout << "reg x: " << reg_x << std::endl;
            special_cycle_reg_values.emplace_back(cycle*reg_x);
        }
    }
    void execute_instruction(Instruction instruction)
    {
        switch(instruction.code)
        {
        case(NOOP):
            increment_cycle();
        break;
        case(ADD):
            increment_cycle();
            increment_cycle();
            reg_x += instruction.value.value_or(0);
        break;
        }
    }
};

int main()
{
    auto tokenizer = FileTokenizer("../input/day10.txt", '\n');
    Circuit circuit;
    for(std::string inst_str = tokenizer.next().value_or("");
         !inst_str.empty();
         inst_str = tokenizer.next().value_or(""))
    {
        Instruction instruction = Instruction(tokenize_string(std::move(inst_str), " "));
        circuit.execute_instruction(instruction);
    }
    int part1 = std::accumulate(circuit.special_cycle_reg_values.cbegin(),
                circuit.special_cycle_reg_values.cend(),
                0);
    std::cout << "part1: " << part1 << std::endl;

    for(const auto& row : circuit.crt)
    {
        for(const auto& pixle : row)
        {
            std::cout <<(char)pixle;
        }
        std::cout << std::endl;
    }
    return 1;
}