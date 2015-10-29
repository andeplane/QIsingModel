#ifndef RANDOM2_H
#define RANDOM2_H
#include <climits>
#include <cmath>

static unsigned long x=123456789, y=362436069, z=521288629;
class Random2
{
public:
    Random2() { }
    double oneOverLongmax = 1.0/ULONG_MAX;
    inline unsigned long nextLong();
    inline unsigned int nextUInt(const int &min, const int &max);
    inline double nextDouble();
};

unsigned long Random2::nextLong() {          //period 2^96-1
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}
unsigned int Random2::nextUInt(const int &min, const int &max) { return min + (max+1-min)*nextDouble(); }
double Random2::nextDouble() { return nextLong()*oneOverLongmax; }

#endif // RANDOM2_H
