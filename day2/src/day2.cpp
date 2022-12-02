#include<array>
#include<numeric>
#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<optional>
#include<string>
#include <FileTokenizer.h>

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
    Game(const std::string& symbols_str);
    Result getResult();
    uint32_t calcTotalPoints();
};

Game::Game(const std::string& symbols_str) : theirs(them_symbol_from_letter(symbols_str[0])),
                                    mine(me_symbol_from_letter(symbols_str[2]))
{}

Result Game::getResult()
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

uint32_t Game::calcTotalPoints()
{
    return symbol_to_points(mine) + result_to_points(getResult());
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day2.txt", '\n');
    uint32_t total = 0;

    for(std::string game_str = tokenizer.next().value_or("");
         !game_str.empty();
         game_str = tokenizer.next().value_or(""))
    {
        Game game = Game(game_str);
        total += game.calcTotalPoints();
    }

    std::cout << "part1: " << total << std::endl;
    
    return 1;
}