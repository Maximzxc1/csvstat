#include "number.h"

#include <stddef.h>
#include <stdlib.h>

#define MY_LLONG_MAX 9223372036854775807LL
#define MY_LLONG_MIN (-MY_LLONG_MAX - 1)

int parse_int(const char *s, long long *out)
{
    size_t i = 0;
    int negative = 0;
    unsigned long long mag = 0;

    if (s[i] == '+' || s[i] == '-')
    {
        negative = (s[i] == '-');
        i++;
    }
    if (s[i] == '\0')
    {
        return -1;
    }

    unsigned long long limit = (unsigned long long)MY_LLONG_MAX;
    if (negative)
    {
        limit = limit + 1;
    }

    for (; s[i] != '\0'; i++)
    {
        if (s[i] < '0' || s[i] > '9')
        {
            return -1;
        }
        unsigned d = (unsigned)(s[i] - '0');
        if (mag > (limit - d) / 10)
        {
            return -1;
        }
        mag = mag * 10 + d;
    }

    if (!negative)
    {
        *out = (long long)mag;
    }
    else if (mag == (unsigned long long)MY_LLONG_MAX + 1)
    {
        *out = MY_LLONG_MIN;
    }
    else
    {
        *out = -(long long)mag;
    }
    return 0;
}

static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int parse_double(const char *s, double *out)
{
    size_t i = 0;
    size_t digits;

    if (s[i] == '+' || s[i] == '-')
    {
        i++;
    }

    digits = 0;
    while (is_digit(s[i]))
    {
        i++;
        digits++;
    }
    if (digits == 0)
    {
        return -1;
    }

    if (s[i] != '.')
    {
        return -1;
    }
    i++;

    digits = 0;
    while (is_digit(s[i]))
    {
        i++;
        digits++;
    }
    if (digits == 0)
    {
        return -1;
    }

    if (s[i] != '\0')
    {
        return -1;
    }

    *out = strtod(s, NULL);
    return 0;
}