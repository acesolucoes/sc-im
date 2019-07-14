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

struct shortcut* create_shortcut(char *key, char *val);

struct shortcut_list *parse_shortcut_file();

#endif