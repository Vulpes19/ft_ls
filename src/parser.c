#include "structs.h"
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <string.h>
#include "printer.h"


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

size_t    store_entries(const char *directory, t_entry_data *data) {
    size_t size = 0;
    size_t capacity = 5;
    DIR *dir = opendir(directory);
    struct dirent *entry;
    struct stat     statbuf;

    data->entries = (t_entry **)ft_calloc(capacity, sizeof(t_entry *));
    if (!data->entries) {
        perror(ft_strjoin("Failed to allocate memory for t_entry ** in: ", strerror(errno)));
        exit(EXIT_FAILURE);
    }

    if (dir) {
        while ((entry = readdir(dir)) != NULL) {
            char *full_path = ft_strjoin(directory, "/");
            full_path = ft_strjoin(full_path, entry->d_name);
            // printf("name: %s\n", full_path);
            if (stat(full_path, &statbuf) == -1) {
                free_d_ptr(data->entries, size);
                free(data);
                handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to get stat");
                continue;
            }
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


    // free_d_ptr(data->entries, size);
    // free(data);
    return data->size;
}

void parse_flags(t_flags *flags, char *argument) {
    int i = 1;
    int len = ft_strlen(argument);

    while (i < len) {
        char flag = argument[i];

        switch (flag)
        {
            case 'l':
                flags->l_flag = true;
                break;
            case 'a':
                flags->a_flag = true;
                break;
            case 'R':
                flags->R_flag = true;
                break;
            case 'r':
                flags->r_flag = true;
                break;
            case 't':
                flags->t_flag = true;
                break;
            default:
                break;
        }
        
        i += 1;
    }
}