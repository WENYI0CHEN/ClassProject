#include <random>

float randFloat(float min, float max)
{
    return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

int randInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

float clamp(float value, float min, float max)
{
    if( value < min ) return min;
    if( value > max ) return max;
    return value;
}
