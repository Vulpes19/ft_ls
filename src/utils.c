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
    char *full_message = ft_strjoin(error_msg, "line: ");
    char *c = ft_itoa(line_number);
    full_message = ft_strjoin(full_message, c);
    full_message = ft_strjoin(full_message, ", file: ");
    full_message = ft_strjoin(full_message, file_name);
    full_message = ft_strjoin(full_message, ", function: ");
    full_message = ft_strjoin(full_message, function);
    full_message = ft_strjoin(full_message, ". ");
    perror(ft_strjoin(full_message, strerror(errno)));
    free(full_message);
    exit(EXIT_FAILURE);
}