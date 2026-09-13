#include <stdlib.h>
#include "../Libft/libft.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>

void    free_d_ptr(void *p, int len) {
    int i = 0;
    void **d_ptr = (void **)p;
    
    while (i < len) {
        free(d_ptr[i]);
        i++;
    }
    free(d_ptr);
}

void    handle_error(int line_number, const char *file_name, 
    const char *function, const char *error_msg) 
{
    fprintf(stderr, "%s (line: %d, file: %s, function: %s): %s\n",
            error_msg, line_number, file_name, function, strerror(errno));
    exit(EXIT_FAILURE);
}