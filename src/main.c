#include <stdio.h>
#include <stdbool.h>
#include "../Libft/ft_printf.h"
#include "../Libft/libft.h"
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "structs.h"

void    free_d_ptr(void *p, int len) {
    int i = 0;
    void **d_ptr = (void **)p;
    
    while (i < len) {
        // if ((struct entry**)d_ptr) {
        //     entry *temp = (struct entry*)d_ptr[i]; 
        //     free(temp->name);
        // }
        free(d_ptr[i]);
        i++;
    }
    free(d_ptr);
}

void copy_entries(t_entry **new_entries, t_entry **entries, int size) {
    int i = 0;

    while (i < size) {
        new_entries[i] = (t_entry *)malloc(sizeof(t_entry));
        new_entries[i]->name = ft_strdup(entries[i]->name);
        free(entries[i]->name);

        i++;
    }
}

void    store_entries(void) {
    int size = 0;
    int capacity = 5;
    DIR *dir = opendir(".");
    struct dirent *entry;

    t_entry **entries = (t_entry **)ft_calloc(sizeof(t_entry *), capacity);

    if (dir) {
        while ((entry = readdir(dir)) != NULL) {
            if (size == capacity) {
                capacity *= 2;
                t_entry **new_entries = (t_entry **)ft_calloc(sizeof(t_entry *), capacity);
                copy_entries(new_entries, entries, size);
                free_d_ptr(entries, size);
                entries = new_entries;
            }
            entries[size] = (t_entry *)malloc(sizeof(t_entry));
            entries[size]->name = ft_strdup(entry->d_name);

            size++;
        }
    }
    
    closedir(dir);

    int j = 0;
    while (entries[j]) {
        printf("inside: %s\n", entries[j]->name);
        j++;
    }
}

void parse_flags(checker *ch, char *argument) {
    int i = 1;
    int len = ft_strlen(argument);

    while (i < len) {
        char flag = argument[i];

        switch (flag)
        {
            case 'l':
                ch->l_flag = true;
                break;
            case 'a':
                ch->a_flag = true;
                break;
            case 'R':
                ch->R_flag = true;
                break;
            case 'r':
                ch->r_flag = true;
                break;
            case 't':
                ch->t_flag = true;
                break;
            default:
                break;
        }
        
        i += 1;
    }
}

// void    parse_arg(char **target_dir, char *argument) {
//     if (opendir(argument) != NULL)

// }

int main(int ac, char **av)
{
    int i = 0;

    checker ch = {
        false,
        false,
        false,
        false,
        false
    };

    while (i < ac) {
        ft_printf("%s\n", av[i]);
        if (av[i][0] == '-')
            parse_flags(&ch, av[i]);
        // else
        //     parse_arg(&target_dir, av[i]);
        i++;
    }
    store_entries();

    // printf("%d %d %d %d %d\n", (int)ch.a_flag, (int)ch.l_flag, (int)ch.R_flag, (int)ch.r_flag, (int)ch.t_flag);
    return (0);
}