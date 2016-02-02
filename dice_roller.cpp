#include "dice_roller.h"

DiceRoller::DiceRoller(int from, int to, int seed) :
    generator(std::default_random_engine(seed)),
    distribution(std::uniform_int_distribution<int>(from,to))
{

}

DiceRoller::~DiceRoller()
{
//    delete generator;
//    delete distribution;
}

int DiceRoller::generate()
{
    return distribution.operator()(generator);
}
