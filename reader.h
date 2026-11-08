#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stddef.h>

typedef struct
{
    char *line;
    size_t len;
    size_t cap;
    char **fields;
    size_t count;
    size_t fcap;
} Row;

void row_init(Row *row);
void row_free(Row *row);
int read_row(FILE *fp, Row *row);

#endif