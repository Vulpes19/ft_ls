#include "parser.h"

enum sort_order {
    S_REVERSE, //Reverse sort order. Works with the current sort key (name, time, size, etc)
    S_DATE, // Sort by modification time (newest first)
    S_DATE_REVERSE
};

bool    compare(struct stat left, struct stat right, enum sort_order order) {
    switch (order)
    {
    case S_REVERSE:
        /* code */
        break;
    
    default:
        break;
    }
} 

void    merge(t_entry_data *data, int left, int middle, int right, enum sort_order order) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    t_entry *left_arr[n1], *right_arr[n2];

    for (i = 0; i < n1; i++) {
        left_arr[i] = data->entries[left + i];
    }
    for (j = 0; j < n2; j++) {
        right_arr[i] = data->entries[middle + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (compare(left_arr[i]->stat, right_arr[j]->stat, order)) {
            data->entries[k] = 
        }
    }
}

void    sort_entries(t_entry_data *data, int left, int right, enum sort_order order) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        sort_entries(data, left, middle, order);
        sort_entries(data, middle + 1, right, order);

        merge(data, left, middle, right, order);
    }
}

void    print_output(checker *ch, t_entry_data *data) {
    if (ch->r_flag && ch->t_flag)
        sort_entries(data, 0, data->size, S_DATE_REVERSE);
    else if (ch->r_flag)
        sort_entries(data, 0, data->size, S_REVERSE);
    else if (ch->t_flag)
        sort_entries(data, 0, data->size, S_DATE);
    
}