#include<array>
#include<iostream>
#include<cassert>
#include<cstdint>
#include<cstdlib>
#include<stack>
#include<deque>
#include<vector>
#include <FileTokenizer.h>

static const constexpr size_t NUM_STACKS = 9;
static const constexpr size_t CRATE_CHAR_CHUNK_SIZE = 4;
static const constexpr char CRATE_DETECTION_CHAR = '[';

struct CrateMoveInstruction
{
    size_t num_crates;
    size_t stack_from;
    size_t stack_to;
    CrateMoveInstruction(const std::string& instruction_str)
    {
        sscanf(instruction_str.c_str(),
            "move %lu from %lu to %lu",
             &num_crates,
             &stack_from,
             &stack_to);
        stack_from--;
        stack_to--;
    }
};

struct Crates
{
    std::array<std::stack<char>,NUM_STACKS> m_crate_stacks;
    Crates(std::array<std::deque<char>,NUM_STACKS>&& crates_dq_array)
    {
        for(size_t i = 0; i < crates_dq_array.size(); i++)
        {
            for(const char& crate_char : crates_dq_array[i])
            {
                m_crate_stacks[i].push(crate_char);
            }
        }
    }
    void execute_instruction(const CrateMoveInstruction& instruction)
    {
        for(size_t i = 0; i < instruction.num_crates; i++)
        {
            char crate_to_move = m_crate_stacks[instruction.stack_from].top();
            m_crate_stacks[instruction.stack_from].pop();
            m_crate_stacks[instruction.stack_to].push(crate_to_move);
        }
    }
    void execute_instruction_9001(const CrateMoveInstruction& instruction)
    {
        std::stack<char> temp_stack;
        for(size_t i = 0; i < instruction.num_crates; i++)
        {
            char crate_to_move = m_crate_stacks[instruction.stack_from].top();
            m_crate_stacks[instruction.stack_from].pop();
            temp_stack.push(crate_to_move);
        }
        for(size_t i = 0; i < instruction.num_crates; i++)
        {
            char crate_to_move = temp_stack.top();
            temp_stack.pop();
            m_crate_stacks[instruction.stack_to].push(crate_to_move);
        }
    }

};



std::vector<std::string> chunk_string_by(const std::string& str, size_t chunk_size)
{
    std::vector<std::string> chunked_strings;
    std::string string_chunk = "";
    for(size_t i = 0; i < str.size(); i++)
    {
        string_chunk += str.c_str()[i];
        if(i % chunk_size == chunk_size-1)
        {
            chunked_strings.emplace_back(std::move(string_chunk));
            string_chunk = "";
        }
    }
    chunked_strings.emplace_back(std::move(string_chunk));

    return chunked_strings;
}

void add_to_crate_builder(std::array<std::deque<char>,NUM_STACKS>& builder,
     const std::string& crates_str)
{
    auto chunked_crates_str = chunk_string_by(crates_str, 4);
    assert(chunked_crates_str.size() == 9);
    for(size_t i = 0; i < chunked_crates_str.size(); i++)
    {
        const auto& chunked_str = chunked_crates_str[i];
        if(chunked_str.find(CRATE_DETECTION_CHAR) != std::string::npos)
        {
            char crate_name = '\0';
            sscanf(chunked_str.c_str(), "[%c] ", &crate_name);
            builder[i].push_front(crate_name);
        }
    }
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day5.txt", '\n');
    std::array<std::deque<char>,NUM_STACKS> crates_builder;

    for(std::string crate_str = tokenizer.next().value_or("");
         crate_str != "";
         crate_str = tokenizer.next().value_or(""))
    {
        if(crate_str.find(CRATE_DETECTION_CHAR) != std::string::npos)
        {
            add_to_crate_builder(crates_builder,crate_str);
        }
    }

    auto crates = Crates(std::move(crates_builder));
    auto crates_2 = crates;

    for(std::string instruction_str = tokenizer.next().value_or("");
         !instruction_str.empty();
         instruction_str = tokenizer.next().value_or(""))
    {
        auto instruction = CrateMoveInstruction(instruction_str);
        crates.execute_instruction(instruction);
        crates_2.execute_instruction_9001(instruction);
    }
    
    std::cout << "part1: ";
    for(const auto& stack : crates.m_crate_stacks)
    {
        std::cout << stack.top();
    }
    std::cout << std::endl;
    std::cout << "part2: ";
    for(const auto& stack : crates_2.m_crate_stacks)
    {
        std::cout << stack.top();
    }
    std::cout << std::endl;
    return 1;
}