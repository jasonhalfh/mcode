#ifndef HANDLE_METHODS_H
#define NANDLE_METHODS_H 1

#include "globle.h"
#include "file_opts.h"


int get_min_start_time(char *dir, int flag);

void calc_area(struct area *a, char *dir);

void calc_density(char *inf, char *of, int gsize, FILE *allfp);
#endif
