#ifndef PARSER_H
#define PARSER_H

#include "utils/dictionary.h"
#include "buffer.h"

struct shortcut {
    char* key;
    struct block* values;
};

struct shortcut_list {
    struct shortcut *shortcut;
    struct shortcut_list* next;
};

struct shortcut_table {
    struct shortcut_list* normal_mode_shortcut_list;
    struct shortcut_list* visual_mode_shortcut_list;
    struct shortcut_list* edit_mode_shortcut_list;
    struct shortcut_list* insert_mode_shortcut_list;
};

struct shortcut* create_shortcut(char *key, char *val);

struct shortcut_table *parse_shortcut_file();

#endif