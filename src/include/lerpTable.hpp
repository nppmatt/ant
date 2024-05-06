/* ant/src/include/lerpTable.hpp */
#ifndef ANT_INCLUDE_LERPTABLE_H_
#define ANT_INCLUDE_LERPTABLE_H_

#include "numerics.hpp"
#include "sintable_0_0001.hpp"
#include "sintable_0_00001.hpp"

#define modd(x, y) ((x) - (int)((x) / (y)) * (y))
#define lerp(w, v1, v2) ((1.0 - (w)) * (v1) + (w) * (v2))

double absd(double a) { *((unsigned long *)&a) &= ~(1UL << 63); return a; }

double sin_table_0_0001_LERP(double x)
{
    x = absd(x);
    x = modd(x, pi2);
    double i = x * 1000.0;
    int index = (int)i;
    return lerp(i - index,        /* weight      */
        sintable_0_0001[index],     /* lower value */
        sintable_0_0001[index + 1]  /* upper value */
        );
}

double sin_table_0_00001_LERP(double x)
{
    x = absd(x);
    x = modd(x, pi2);
    double i = x * 10000.0;
    int index = (int)i;
    return lerp(i - index,        /* weight      */
        sintable_0_00001[index],     /* lower value */
        sintable_0_00001[index + 1]  /* upper value */
        );
}

#endif
/* ant/src/include/lerpTable.hpp */
