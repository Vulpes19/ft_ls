#pragma once

#include <sys/stat.h>
#include "../Libft/ft_printf.h"
#include "../Libft/libft.h"
#include <stdbool.h>

void    free_d_ptr(void *p, int len);
void    handle_error(int line_number, const char *file_name, 
    const char *function, const char *error_msg);

typedef struct {
    bool l_flag;
    bool R_flag;
    bool a_flag;
    bool r_flag;
    bool t_flag;
    int dir_nbr;
} t_flags;

typedef struct {
    char *name;
    struct stat     stat;
} t_entry;

typedef struct {
    t_entry **entries;
    size_t size;
} t_entry_data;