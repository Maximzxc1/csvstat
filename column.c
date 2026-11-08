#include "column.h"

#define BASE 4294967296LL
#define TWO_POW_53 9007199254740992.0

static int is_integral(double v)
{
    if (v >= TWO_POW_53 || v <= -TWO_POW_53)
    {
        return 1;
    }
    return v == (double)(long long)v;
}

static void add_to_double_stats(Column *col, double v)
{
    if (col->dcount == 0 || v < col->dmin)
    {
        col->dmin = v;
    }
    if (col->dcount == 0 || v > col->dmax)
    {
        col->dmax = v;
    }
    col->dcount++;
    col->dsum += v;
    if (v > 0)
    {
        col->dpositive++;
    }
    else if (v < 0)
    {
        col->dnegative++;
    }
    if (is_integral(v))
    {
        col->dinteger++;
    }
}

void column_add(Column *col, long long v)
{
    if (col->count == 0 || v < col->min)
    {
        col->min = v;
    }
    if (col->count == 0 || v > col->max)
    {
        col->max = v;
    }
    col->count++;
    if (v > 0)
    {
        col->positive++;
    }
    else if (v < 0)
    {
        col->negative++;
    }
    if (v % 2 == 0)
    {
        col->even++;
    }

    long long hi = v / BASE;
    long long lo = v % BASE;
    if (lo < 0)
    {
        lo += BASE;
        hi -= 1;
    }

    col->sum_hi += hi;
    col->sum_lo += lo;
    if (col->sum_lo >= BASE)
    {
        col->sum_lo -= BASE;
        col->sum_hi += 1;
    }

    add_to_double_stats(col, (double)v);
}

void column_add_double(Column *col, double v)
{
    col->has_double = 1;
    add_to_double_stats(col, v);
}

double column_mean(const Column *col)
{
    long long n = (long long)col->count;

    long long q1 = col->sum_hi / n;
    long long r1 = col->sum_hi % n;
    if (r1 < 0)
    {
        r1 += n;
        q1 -= 1;
    }

    long long rest = r1 * BASE + col->sum_lo;
    long long q2 = rest / n;
    long long r2 = rest % n;

    return (double)(q1 * BASE + q2) + (double)r2 / (double)n;
}

double column_double_mean(const Column *col)
{
    return col->dsum / (double)col->dcount;
}