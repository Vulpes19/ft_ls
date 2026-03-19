#pragma once

#include "structs.h"
#include <dirent.h>
#include <pwd.h>

void    store_entries(checker *ch); // stores entries (directory names, file names etc) in a struct
void    parse_flags(checker *ch, char *argument); // parses the flags (-l, -a, -R etc)
void    copy_entries(t_entry **new_entries, t_entry **entries, int size);
void    print_output(checker *ch, t_entry_data *data);