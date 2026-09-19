#include "ls.h"
#include "utils.h"

void    ls_subdir(t_flags *flags, t_entry **entries, size_t size) {

    for (size_t i = 0; i < size; i++) {
        if (strcmp(entries[i]->name, ".") == 0 || strcmp(entries[i]->name, "..") == 0 || 
            (!flags->a_flag && entries[i]->name[0] == '.'))
            continue ;
        if (entries[i]->type == DT_DIR) {
            printf("\n%s:\n", entries[i]->full_path);
            ls_dir(flags, entries[i]->full_path);
        }
    }
}

void    ls_dir(t_flags *flags, char *path) {

    t_entry_data *data = NULL;
    size_t size = 0;
    int max_size_width = 0;
    int max_nlink_width = 0;

    data = (t_entry_data *)malloc(sizeof(t_entry_data));
    if (!data) {
        handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to allocate memory for t_entry_data * : ");
    }
    data->total = 0;

    size = store_entries(path, data, flags, &max_size_width, &max_nlink_width);
    
    data->max_size_width = max_size_width;
    data->max_nlink_width = max_nlink_width;
    
    sort_entries(flags, data);
    print_output(flags, path, data);

    
    if (flags->R_flag)
        ls_subdir(flags, data->entries, size);
    free_d_ptr(data->entries, size);
    free(data);
}