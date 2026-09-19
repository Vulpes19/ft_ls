#include <stdlib.h>
#include "../Libft/libft.h"
#include <errno.h>
#include <string.h>
#include "structs.h"
#include <stdio.h>

void    free_d_ptr(t_entry **entries, int len) {
    int i = 0;
    
    if (!entries)
        return;

    while (i < len) {
        if (entries[i]) {
            // 1. Free the dynamically allocated strings inside the struct
            free(entries[i]->name);
            free(entries[i]->full_path);
            
            // 2. Free the struct itself
            free(entries[i]);
        }
        i++;
    }
    // 3. Free the array of pointers
    free(entries);
}

void    handle_error(int line_number, const char *file_name, 
    const char *function, const char *error_msg) 
{
    fprintf(stderr, "%s (line: %d, file: %s, function: %s): %s\n",
            error_msg, line_number, file_name, function, strerror(errno));
    exit(EXIT_FAILURE);
}