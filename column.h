#ifndef COLUMN_H
#define COLUMN_H

#include <stddef.h>

typedef struct
{
    char *name;

    size_t count;
    long long min;
    long long max;
    size_t positive;
    size_t negative;
    size_t even;
    long long sum_hi;
    long long sum_lo;

    int has_double;
    size_t dcount;
    double dmin;
    double dmax;
    double dsum;
    size_t dpositive;
    size_t dnegative;
    size_t dinteger;
} Column;

void column_add(Column *col, long long v);
void column_add_double(Column *col, double v);
double column_mean(const Column *col);
double column_double_mean(const Column *col);

#endif