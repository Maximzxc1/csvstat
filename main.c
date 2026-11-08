#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "column.h"
#include "number.h"
#include "output.h"
#include "reader.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "csvstat: не указан режим\n");
        return 2;
    }
    if (strcmp(argv[1], "--help") == 0)
    {
        if (argc != 2)
        {
            fprintf(stderr, "csvstat: лишние аргументы после --help\n");
            return 2;
        }
        printf("Использование:\n");
        printf("  csvstat --help\n");
        printf("  csvstat schema <csv-file>\n");
        printf("  csvstat stats <csv-file>\n");
        return 0;
    }
    if (strcmp(argv[1], "schema") != 0 && strcmp(argv[1], "stats") != 0)
    {
        fprintf(stderr, "csvstat: неизвестный режим '%s'\n", argv[1]);
        return 2;
    }
    if (argc != 3)
    {
        fprintf(stderr, "csvstat: нужно указать ровно один файл\n");
        return 2;
    }

    FILE *fp = fopen(argv[2], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "csvstat: не удалось открыть файл '%s'\n", argv[2]);
        return 1;
    }

    Row row;
    row_init(&row);
    Column *cols = NULL;
    size_t ncols = 0;
    size_t rows = 0;
    int rc = 0;

    int status = read_row(fp, &row);
    if (status == 0)
    {
        fprintf(stderr, "csvstat: файл пуст\n");
        rc = 1;
    }
    else if (status < 0)
    {
        rc = 1;
    }
    else
    {
        cols = calloc(row.count, sizeof(Column));
        if (cols == NULL)
        {
            fprintf(stderr, "csvstat: недостаточно памяти\n");
            rc = 1;
        }
        else
        {
            ncols = row.count;
            for (size_t i = 0; i < ncols; i++)
            {
                size_t n = strlen(row.fields[i]) + 1;
                cols[i].name = malloc(n);
                if (cols[i].name == NULL)
                {
                    fprintf(stderr, "csvstat: недостаточно памяти\n");
                    rc = 1;
                    break;
                }
                strcpy(cols[i].name, row.fields[i]);
            }
        }
    }

    if (rc == 0)
    {
        while ((status = read_row(fp, &row)) == 1)
        {
            if (row.count != ncols)
            {
                fprintf(stderr, "csvstat: в строке %d полей %d, а столбцов %d\n",
                        (int)(rows + 2), (int)row.count, (int)ncols);
                rc = 1;
                break;
            }
            for (size_t i = 0; i < ncols; i++)
            {
                long long iv;
                double dv;
                if (parse_int(row.fields[i], &iv) == 0)
                {
                    column_add(&cols[i], iv);
                }
                else if (parse_double(row.fields[i], &dv) == 0)
                {
                    column_add_double(&cols[i], dv);
                }
                else
                {
                    fprintf(stderr, "csvstat: в строке %d значение '%s' не является числом\n",
                            (int)(rows + 2), row.fields[i]);
                    rc = 1;
                    break;
                }
            }
            if (rc != 0)
            {
                break;
            }
            rows++;
        }
        if (status < 0)
        {
            rc = 1;
        }
    }

    if (rc == 0)
    {
        if (strcmp(argv[1], "stats") == 0)
        {
            print_stats(cols, ncols, rows);
        }
        else
        {
            print_schema(cols, ncols, rows);
        }
    }

    for (size_t i = 0; i < ncols; i++)
    {
        free(cols[i].name);
    }
    free(cols);
    row_free(&row);
    fclose(fp);
    return rc;
}