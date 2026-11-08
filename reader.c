#include "reader.h"

#include <stdlib.h>

void row_init(Row *row)
{
    row->line = NULL;
    row->len = 0;
    row->cap = 0;
    row->fields = NULL;
    row->count = 0;
    row->fcap = 0;
}

void row_free(Row *row)
{
    free(row->line);
    free(row->fields);
    row_init(row);
}

static int push_char(Row *row, char c)
{
    if (row->len + 1 >= row->cap)
    {
        size_t new_cap = (row->cap == 0) ? 16 : row->cap * 2;
        char *p = realloc(row->line, new_cap);
        if (p == NULL)
        {
            fprintf(stderr, "csvstat: недостаточно памяти\n");
            return -1;
        }
        row->line = p;
        row->cap = new_cap;
    }
    row->line[row->len] = c;
    row->len++;
    return 0;
}

static int push_field(Row *row, char *start)
{
    if (row->count == row->fcap)
    {
        size_t new_fcap = (row->fcap == 0) ? 4 : row->fcap * 2;
        char **p = realloc(row->fields, new_fcap * sizeof(char *));
        if (p == NULL)
        {
            fprintf(stderr, "csvstat: недостаточно памяти\n");
            return -1;
        }
        row->fields = p;
        row->fcap = new_fcap;
    }
    row->fields[row->count] = start;
    row->count++;
    return 0;
}

int read_row(FILE *fp, Row *row)
{
    int c;

    row->len = 0;
    row->count = 0;

    c = getc(fp);
    while (c != EOF && c != '\n')
    {
        if (push_char(row, (char)c) != 0)
        {
            return -1;
        }
        c = getc(fp);
    }
    if (ferror(fp))
    {
        fprintf(stderr, "csvstat: ошибка чтения файла\n");
        return -1;
    }
    if (c == EOF && row->len == 0)
    {
        return 0;
    }
    row->line[row->len] = '\0';

    char *start = row->line;
    for (size_t i = 0; i <= row->len; i++)
    {
        if (row->line[i] == ',' || row->line[i] == '\0')
        {
            row->line[i] = '\0';
            if (push_field(row, start) != 0)
            {
                return -1;
            }
            start = row->line + i + 1;
        }
    }
    return 1;
}