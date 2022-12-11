#include<iostream>
#include<algorithm>
#include<functional>
#include<optional>
#include<cstdint>
#include<cstdlib>
#include<deque>
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

struct Operation
{
    std::optional<uint64_t> constant;
    bool addition;
    Operation() : constant(), addition()
    {}
    
    uint64_t operator() (uint64_t old)
    {
        uint64_t c = constant == std::nullopt ? old : constant.value_or(0);
        if(addition)
        {
            return c + old;
        }
        return c * old;
    }
};

struct Pred
{
    uint64_t div;
    uint64_t true_monkey;
    uint64_t false_monkey;
    Pred() : div(0), true_monkey(0), false_monkey(0)
    {}
    uint64_t operator() (uint64_t test_num)
    {
        return (test_num % div == 0) ? true_monkey : false_monkey;
    }
};

struct Monkey
{
    std::deque<uint64_t> items;

    Operation operation;
    Pred predicate;

    size_t num_inspected;
    int inspect_worry_div;
    int lcm;
    Monkey() : items(), operation(), predicate(), num_inspected(0), inspect_worry_div(3), lcm(1)
    {}
    void print()
    {
        for(const auto& item : items)
        {
            std::cout << item << ", ";
        }
        std::cout << std::endl;
    };
    void parse_items(std::string&& items_str)
    {
        auto item_tokens = tokenize_string(std::move(items_str), " ");
        for(size_t i = 2; i < item_tokens.size(); i++)
        {
            uint64_t worry = 0;
            sscanf(item_tokens[i].c_str(), "%lu,", &worry);
            items.emplace_back(worry);
        }
    }

    void parse_operation(std::string&& operation_str)
    {
        auto oper_tokens = tokenize_string(std::move(operation_str), " ");
        operation.constant = std::nullopt;
        operation.addition = oper_tokens[4] == "*" ? false : true;
        if(oper_tokens.back() != "old")
        {
            uint64_t c = 0;
            sscanf(oper_tokens.back().c_str(),"%lu", &c);
            operation.constant = c;
        }
    }
    void parse_pred(std::vector<std::string>&& pred_strs)
    {
        auto div_str_tokens = tokenize_string(std::move(pred_strs[0]), " ");
        auto true_monkey_tokens = tokenize_string(std::move(pred_strs[1]), " ");
        auto false_monkey_tokens = tokenize_string(std::move(pred_strs[2]), " ");

        sscanf(div_str_tokens.back().c_str(), "%lu", &predicate.div);
        sscanf(true_monkey_tokens.back().c_str(), "%lu", &predicate.true_monkey);
        sscanf(false_monkey_tokens.back().c_str(), "%lu", &predicate.false_monkey);
    }

    int inspect_item(int item_worry)
    {
        num_inspected++;
        if(inspect_worry_div  != 1)
        {
            return operation(item_worry) / inspect_worry_div;
        }
        return operation(item_worry) % lcm;
    }

};
void monkey_turn(Monkey& current_monkey, std::vector<Monkey>& monkeys)
{
    size_t item_size = current_monkey.items.size();
    for(size_t i = 0; i < item_size; i++)
    {
        uint64_t item_worry = current_monkey.items.front();
        current_monkey.items.pop_front();
        uint64_t new_worry = current_monkey.inspect_item(item_worry);
        size_t new_monkey_idx = current_monkey.predicate(new_worry);
        monkeys[new_monkey_idx].items.emplace_back(new_worry);
    }
}


int main()
{
    auto tokenizer = FileTokenizer("../input/day11.txt", '\n');

    std::vector<Monkey> monkeys;
    std::vector<Monkey> part2_monkeys;
    size_t num_rounds_part_1 = 20;
    size_t num_rounds_part_2 = 10000;
    for(std::string pairs_str = tokenizer.next().value_or("");
         !pairs_str.empty();
         pairs_str = tokenizer.next().value_or(""))
    {
        Monkey monkey;
        monkey.parse_items(tokenizer.next().value_or(""));
        monkey.parse_operation(tokenizer.next().value_or(""));
        std::vector<std::string> pred_strs = {tokenizer.next().value_or(""),
                     tokenizer.next().value_or(""),
                     tokenizer.next().value_or("")};
        monkey.parse_pred(std::move(pred_strs));
        std::string unused = tokenizer.next().value_or("");
        monkeys.push_back(monkey);
    }
    int lcm = 1;
    for(auto& monkey : monkeys)
    {
        lcm *= monkey.predicate.div;
    }
    for(auto& monkey : monkeys)
    {
        monkey.lcm = lcm;
    }
    part2_monkeys = monkeys;
    for(size_t i = 0; i < num_rounds_part_1; i++)
    {
        for(auto& monkey : monkeys)
        {
            monkey_turn(monkey, monkeys);
        }
    }
    std::sort(monkeys.begin(),monkeys.end(),
        [](const Monkey& m1, const Monkey& m2){return m1.num_inspected > m2.num_inspected;});
    std::cout << "part1: " << monkeys[0].num_inspected * monkeys[1].num_inspected << std::endl;

    //part2
    for(auto& monkey : part2_monkeys)
    {
        monkey.inspect_worry_div = 1;
    }
    for(size_t i = 0; i < num_rounds_part_2; i++)
    {
        for(auto& monkey : part2_monkeys)
        {
            monkey_turn(monkey, part2_monkeys);
        }
    }
    std::sort(part2_monkeys.begin(),part2_monkeys.end(),
        [](const Monkey& m1, const Monkey& m2){return m1.num_inspected > m2.num_inspected;});
    std::cout << "part2: " << part2_monkeys[0].num_inspected * part2_monkeys[1].num_inspected << std::endl;
    return 1;
}