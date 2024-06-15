#include "Maths.h"

#include <cmath>

double Gin::Maths::DegsToRads(double deg)
{
    return deg * (M_PI / 180);
}

double Gin::Maths::RadsToDegs(double rad)
{
    return rad * (180 / M_PI);
}

int Gin::Maths::Sign(int x)
{
    return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}
