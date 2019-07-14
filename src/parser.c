#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "buffer.h"
#include "utils/dictionary.h"
#include "macros.h"

char * trim( char *beg, char *end)
{
    while( isspace(*beg) ) beg++;
    if( beg - end >= 0 ) return NULL;

    while(isspace(*end)) end--;

    int len = end-beg + 1;
    char * keyword = malloc(len);
    strncpy(keyword, beg, len);

    return keyword;
}

struct shortcut* create_shortcut(char *value, char *val)
{
    struct block* buf = create_buf();

    char *ctrlMod = strchr(val, '+');
    int ctlHandled = ctrlMod == NULL;
    char* c = !ctlHandled ? ctrlMod+1 : val;

    char *end = val + strlen(val);
    // ERROR
    if( c == end )
    {
        exit(1);
    }

    while( c != end )
    {
        if( !ctlHandled )
        {
            addto_buf(buf, ctl(*c) );
            ctlHandled = 1;
        }
        else
        {
            addto_buf(buf, *c );
        }
        
        c++;
    }
    
    struct shortcut* l = (struct shortcut*) malloc (sizeof (struct shortcut));
    l->key = value;
    l->values = buf;

    return l; 
}

struct shortcut_list* create_shortcut_list()
{
    struct shortcut_list* l = (struct shortcut_list*) malloc (sizeof (struct shortcut_list));
    l->shortcut = NULL;
    l->next = NULL;

    return l;
}

struct shortcut_table* create_shortcut_table()
{
    struct shortcut_table* table = (struct shortcut_table*) malloc (sizeof (struct shortcut_table));
    table->edit_mode_shortcut_list      = create_shortcut_list();
    table->insert_mode_shortcut_list    = create_shortcut_list();
    table->normal_mode_shortcut_list    = create_shortcut_list();
    table->visual_mode_shortcut_list    = create_shortcut_list();

    return table;
}

struct shortcut_list* add_shortcut(struct shortcut_list* l, char *key, char *val)
{
    printf("%s %s!\n", key, val);
    struct shortcut* shortcut = create_shortcut(key, val);
    
    if( l->shortcut == NULL )
    {
        l->shortcut = shortcut;
        return l;
    }

    struct shortcut_list *nl = create_shortcut_list();
    nl->shortcut = shortcut;


    if( strcmp(key, l->shortcut->key) <= 0 )
    {
        nl->next = l;
        return nl;
    }

    struct shortcut_list *current = l;
    struct shortcut_list *next = l->next;
    while( next )
    {
        if( strcmp(key, next->shortcut->key) <= 0 )
        {   
            break;
        }

        current = next;
        next = next->next;
    }

    current->next = nl;
    nl->next = next;

    return l;
}

struct shortcut_table *parse_shortcut_file()
{
    struct shortcut_table* table = create_shortcut_table();

    struct shortcut_list ** shortcut_list;

    // char * k, *v
    char line[257] = {}; // 0 padded array
    FILE *shortcut_file = fopen("shortcuts", "r");

    if( !shortcut_file )
    {
        fprintf(stderr, "error");
        char destiny[255] = {};
        char* homepath = getenv("HOME");
        strcat(destiny, homepath);
        strcat(destiny, "/.sc-shortcuts");
        puts(destiny);
        
        shortcut_file = fopen(destiny, "r");
    }

    if( !shortcut_file )
    {
        return table;
    }

    char * comment_str, * divider_str;

    while( fgets(line, 256, shortcut_file) )
    {
        comment_str = strchr(line, '#');
        if( comment_str == line) continue;

        divider_str = strchr(line, ':');

        if( !divider_str ) continue;

        int end_pos = comment_str ? (comment_str-1) - line : strlen(line) - 1;

        if( divider_str - line >= end_pos || divider_str == line ) continue; // invalid line

        char *key_part = trim(line, divider_str-1);
    
        char *key_divider_str = strchr(key_part, ',');
        char *key;

        if( !key_divider_str )
        {
            *shortcut_list = table->normal_mode_shortcut_list;
            key  = key_part; 
        }
        else
        {
            char * destiny_shortcut_list = trim(key_part, key_divider_str-1 );
            key = trim(key_divider_str+1, key_part + strlen(key_part) - 1);

            printf("%s - %s\n", destiny_shortcut_list, key);

            if( !strcmp(destiny_shortcut_list, "normal") )
            {
                shortcut_list = &table->normal_mode_shortcut_list;
            }
            else if( !strcmp(destiny_shortcut_list, "insert") )
            {
                shortcut_list = &table->insert_mode_shortcut_list;
            }
            else if( !strcmp(destiny_shortcut_list, "edit") )
            {
                shortcut_list = &table->edit_mode_shortcut_list;
            }
            else if( !strcmp(destiny_shortcut_list, "visual") )
            {
                shortcut_list = &table->visual_mode_shortcut_list;
            }
            else // fallback (TODO: emit a warning)
            {
                shortcut_list = &table->normal_mode_shortcut_list;
            }
        }

        char *value_ot_begin =  divider_str+1;
        char *comma_sep;

        while( (comma_sep = strchr(value_ot_begin, ',')) )
        {
            char * value = trim(value_ot_begin, comma_sep - 1);
            value_ot_begin = comma_sep + 1;    
            *shortcut_list = add_shortcut(*shortcut_list, key, value);
        }

        char *value = trim(value_ot_begin, line + end_pos);

        // put(shortcut_dict, value, key_ot);
        *shortcut_list = add_shortcut(*shortcut_list, key, value);
    }

    return table;   
}