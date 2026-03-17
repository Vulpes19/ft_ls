#include "structs.h"
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <string.h>


void copy_entries(t_entry **new_entries, t_entry_data *data, int size) {
    int i = 0;

    while (i < size) {
        new_entries[i] = (t_entry *)malloc(sizeof(t_entry));
        new_entries[i]->name = ft_strdup(data->entries[i]->name);
        new_entries[i]->stat = data->entries[i]->stat;
        free(data->entries[i]->name);;

        i++;
    }
}

void    store_entries(void) {
    int size = 0;
    int capacity = 5;
    DIR *dir = opendir(".");
    struct dirent *entry;
    t_entry_data *data;
    struct stat     statbuf;

    data = (t_entry_data *)malloc(sizeof(t_entry_data *));
    data->entries = (t_entry **)ft_calloc(capacity, sizeof(t_entry *));
    if (!data->entries) {
        perror(ft_strjoin("Failed to allocate memory for t_entry ** in: ", strerror(errno)));
        exit(EXIT_FAILURE);
    }

    if (dir) {
        while ((entry = readdir(dir)) != NULL) {
            if (stat(entry->d_name, &statbuf) == -1) {
                printf("stat failed\n");
                continue;
            }
            printf("%d %d\n", size, capacity);
            if (size == capacity) {
                capacity *= 2;
                t_entry **new_entries = (t_entry **)ft_calloc(capacity, sizeof(t_entry *));
                if (!new_entries) {
                    free_d_ptr(data->entries, size);
                    free(data);
                    handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to allocate memory for t_entry ** : ");
                }
                copy_entries(new_entries, data, size);
                free_d_ptr(data->entries, size);
                data->entries = new_entries;
            }
            data->entries[size] = (t_entry *)malloc(sizeof(t_entry));
            if (!data->entries[size]) {
                free(data);
                handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to allocate memory for t_entry *: ");
            }
            data->entries[size]->name = ft_strdup(entry->d_name);
            if (!data->entries[size]->name) {
                free_d_ptr(data->entries, size);
                free(data);
                handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to allocate memory for char *: ");
            }
            data->entries[size]->stat = statbuf;
            size++;
        }
        closedir(dir);
        data->size = size;
    }
    

    int j = 0;
    while (j < data->size) {
        printf("inside: %s\n", data->entries[j]->name);
        printf("size: %d\n", data->entries[j]->size);
        // printf("%hu\n", entries[j]->stat.st_mode);
        // printf("%d\n", entries[j]->stat.st_nlink);
        // struct passwd  *pwd;
        // if ((pwd = getpwuid(entries[j]->stat.st_uid)) != NULL)
        //     ft_printf(" %s\n", pwd->pw_name);
        // else
        //     printf(" %-8d\n", entries[j]->stat.st_uid);
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