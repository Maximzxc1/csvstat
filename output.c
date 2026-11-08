#include "output.h"

#include <stdio.h>
#include <string.h>

static const char *column_type(const Column *col)
{
    if (col->dcount == 0)
    {
        return "NULL";
    }
    if (col->has_double)
    {
        return "DOUBLE";
    }
    return "INT";
}

static void print_real(const char *key, double v)
{
    char buf[400];
    snprintf(buf, sizeof(buf), "%.6f", v);
    if (strcmp(buf, "-0.000000") == 0)
    {
        strcpy(buf, "0.000000");
    }
    printf("%s %s\n", key, buf);
}

void print_schema(const Column *cols, size_t ncols, size_t rows)
{
    printf("rows %d\n", (int)rows);
    printf("columns %d\n", (int)ncols);
    for (size_t i = 0; i < ncols; i++)
    {
        printf("%d \"%s\" %s\n", (int)i, cols[i].name, column_type(&cols[i]));
    }
}

void print_stats(const Column *cols, size_t ncols, size_t rows)
{
    printf("rows %d\n", (int)rows);
    printf("columns %d\n", (int)ncols);
    for (size_t i = 0; i < ncols; i++)
    {
        const Column *col = &cols[i];
        const char *type = column_type(col);

        printf("\n");
        printf("column %d\n", (int)i);
        printf("name \"%s\"\n", col->name);
        printf("type %s\n", type);

        if (strcmp(type, "NULL") == 0)
        {
            printf("rows %d\n", (int)rows);
        }
        else if (strcmp(type, "INT") == 0)
        {
            printf("nulls 0\n");
            printf("min %lld\n", col->min);
            printf("max %lld\n", col->max);
            print_real("mean", column_mean(col));
            printf("positive %d\n", (int)col->positive);
            printf("negative %d\n", (int)col->negative);
            printf("even %d\n", (int)col->even);
        }
        else
        {
            printf("nulls 0\n");
            print_real("min", col->dmin);
            print_real("max", col->dmax);
            print_real("mean", column_double_mean(col));
            printf("positive %d\n", (int)col->dpositive);
            printf("negative %d\n", (int)col->dnegative);
            printf("integer %d\n", (int)col->dinteger);
        }
    }
}