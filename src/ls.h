#pragma once

#include "parser.h"
#include "sorter.h"
#include "printer.h"

void ls_dir(t_flags *flags, char *path);
void    free_d_ptr(t_entry **entries, int len);