#ifndef OUTPUT_H
#define OUTPUT_H

#include <stddef.h>

#include "column.h"

void print_schema(const Column *cols, size_t ncols, size_t rows);
void print_stats(const Column *cols, size_t ncols, size_t rows);

#endif