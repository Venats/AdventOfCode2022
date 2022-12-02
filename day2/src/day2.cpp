#include<array>
#include<numeric>
#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<optional>
#include<string>
#include<FileTokenizer.h>

enum Symbol : uint32_t
{
    ROCK     =  0,
    PAPER    =  1,
    SCISSORS =  2,
    NUM_SYMBOLS
};

enum Result : uint32_t
{
    LOSE = 0,
    DRAW = 1,
    WIN = 2,
    NUM_RESULTS,
};

std::array<Symbol, Symbol::NUM_SYMBOLS> win_array = {SCISSORS,ROCK,PAPER};
std::array<Symbol, Symbol::NUM_SYMBOLS> lose_array = {PAPER,SCISSORS,ROCK};

Symbol them_symbol_from_letter(char letter)
{
    switch(letter)
    {
    case('A'):
        return Symbol::ROCK;
    break;
    case('B'):
        return Symbol::PAPER;
    break;
    case('C'):
        return Symbol::SCISSORS;
    break;
    default:
        return Symbol::NUM_SYMBOLS;
    break;
    }
}

Symbol me_symbol_from_letter(char letter)
{
    switch(letter)
    {
    case('X'):
        return Symbol::ROCK;
    break;
    case('Y'):
        return Symbol::PAPER;
    break;
    case('Z'):
        return Symbol::SCISSORS;
    break;
    default:
        return Symbol::NUM_SYMBOLS;
    break;
    }
}

Result me_result_from_letter(char letter)
{
    switch(letter)
    {
    case('X'):
        return Result::LOSE;
    break;
    case('Y'):
        return Result::DRAW;
    break;
    case('Z'):
        return Result::WIN;
    break;
    default:
        return Result::NUM_RESULTS;
    break;
    }
}


uint32_t symbol_to_points(Symbol symbol)
{
    return static_cast<uint32_t>(symbol)+1;
}

uint32_t result_to_points(Result result)
{
    return static_cast<uint32_t>(result)*3;
}

struct Game
{
    Symbol theirs;
    Symbol mine;
    Result result_mine;
    Game(const std::string& symbols_str);
};

Game::Game(const std::string& symbols_str) : theirs(them_symbol_from_letter(symbols_str[0])),
                                    mine(me_symbol_from_letter(symbols_str[2])),
                                    result_mine(me_result_from_letter(symbols_str[2]))
{}

Result getResult(Symbol theirs, Symbol mine)
{
    if(win_array[theirs] == mine)
    {
        return LOSE;
    }
    if(theirs == mine)
    {
        return DRAW;
    }
    return WIN;
}

Symbol getSymbol(Symbol theirs, Result mine)
{
    switch(mine)
    {
    case(Result::LOSE):
        return win_array[theirs];
    break;
    case(Result::DRAW):
        return theirs;
    break;
    case(Result::WIN):
        return lose_array[theirs];
    break;
    default:
        return theirs;
    break;
    }
}

uint32_t part1CalcTotalPoints(Symbol theirs, Symbol mine)
{
    return symbol_to_points(mine) + result_to_points(getResult(theirs, mine));
}

uint32_t part2CalcTotalPoints(Symbol theirs, Result mine)
{
    return result_to_points(mine) + symbol_to_points(getSymbol(theirs, mine));
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day2.txt", '\n');
    uint32_t part_1_total = 0;
    uint32_t part_2_total = 0;

    for(std::string game_str = tokenizer.next().value_or("");
         !game_str.empty();
         game_str = tokenizer.next().value_or(""))
    {
        Game game = Game(game_str);
        part_1_total += part1CalcTotalPoints(game.theirs, game.mine);
        part_2_total += part2CalcTotalPoints(game.theirs, game.result_mine);
    }

    std::cout << "part1: " << part_1_total << std::endl;
    std::cout << "part2: " << part_2_total << std::endl;
    
    return 1;
}