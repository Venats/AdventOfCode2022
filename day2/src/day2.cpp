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
    ROCK     =  1,
    PAPER    =  2,
    SCISSORS =  3,
    UNKNOWN
};

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
        return Symbol::UNKNOWN;
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
        return Symbol::UNKNOWN;
    break;
    }
}

uint32_t symbol_to_points(Symbol symbol)
{
    return static_cast<uint32_t>(symbol);
}

struct Game
{
    Symbol theirs;
    Symbol mine;
    Game(const std::string& symbols_str);
    uint32_t getResult();
    uint32_t calcTotalPoints();
};

Game::Game(const std::string& symbols_str) : theirs(them_symbol_from_letter(symbols_str[0])),
                                    mine(me_symbol_from_letter(symbols_str[2]))
{}

uint32_t Game::getResult()
{
    if(theirs == mine)
    {
        //draw
        return 3;
    }
    if(theirs == Symbol::ROCK && mine == Symbol::SCISSORS)
    {
        //loss
        return 0;
    }
    if(theirs == Symbol::SCISSORS && mine == Symbol::ROCK)
    {
        //win
        return 6;
    }
    bool i_win = mine > theirs;
    return i_win ? 6 : 0;
}

uint32_t Game::calcTotalPoints()
{
    return static_cast<uint32_t>(mine) + getResult();
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