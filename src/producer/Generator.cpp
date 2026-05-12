#include "Generator.hpp"

RampingGenerator::RampingGenerator(int minimum, int maximum, int step, int initial)
    : minimum(minimum), maximum(maximum), step(step), value(initial)
{

}

void RampingGenerator::next()
{
    value += step;
    if (value > maximum) 
    {
        value = minimum;
    }
}

int RampingGenerator::advance()
{
    int currentValue = value;
    next();
    return currentValue;
}