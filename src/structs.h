#pragma once

#include <sys/stat.h>

typedef struct {
    bool l_flag;
    bool R_flag;
    bool a_flag;
    bool r_flag;
    bool t_flag;
} checker;

typedef struct {
    char *name;
    struct stat     statbuf;
} t_entry;
