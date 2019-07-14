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

struct shortcut* create_shortcut(char *key, char *val)
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
    l->key = key;
    l->values = buf;

    return l; 
}

struct dictionary *parse_shortcut_file()
{
    struct dictionary *shortcut_dict = create_dictionary();

    // char * k, *v
    char line[257] = {}; // 0 padded array
    FILE *shortcut_file = fopen("shortcuts", "r");

    if( !shortcut_file )
    {
        fputs("It could not possible to open any dictionary file\n", stderr);
        return shortcut_dict;
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

        char *value = trim(line, divider_str-1);
        char *key_begin =  divider_str+1;
        char *comma_sep;

        while( (comma_sep = strchr(key_begin, ',')) )
        {
            char * key = trim(key_begin, comma_sep - 1);
            key_begin = comma_sep + 1;
            put(shortcut_dict, key, value);    
        }

        char *key = trim(key_begin, line + end_pos);

        put(shortcut_dict, key, value);
    }

    return shortcut_dict;
}

struct shortcut_list* create_shortcut_list()
{
    struct shortcut_list* l = (struct shortcut_list*) malloc (sizeof (struct shortcut_list));
    l->shortcut = NULL;
    l->next = NULL;

    return l;
}

struct shortcut_list* add_shortcut(struct shortcut_list* l, char *key, char *val)
{
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

struct shortcut_list *parse_shortcut_file2()
{
    struct shortcut_list * shortcut_list = create_shortcut_list();

    // char * k, *v
    char line[257] = {}; // 0 padded array
    FILE *shortcut_file = fopen("shortcuts", "r");

    if( !shortcut_file )
    {
        fputs("It could not possible to open any dictionary file\n", stderr);
        return shortcut_list;
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

        char *value = trim(line, divider_str-1);
        char *key_begin =  divider_str+1;
        char *comma_sep;

        while( (comma_sep = strchr(key_begin, ',')) )
        {
            char * key = trim(key_begin, comma_sep - 1);
            key_begin = comma_sep + 1;
            // put(shortcut_dict, key, value);    
            shortcut_list = add_shortcut(shortcut_list, value, key);
        }

        char *key = trim(key_begin, line + end_pos);

        // put(shortcut_dict, key, value);
        shortcut_list = add_shortcut(shortcut_list, value, key);
    }

    return shortcut_list;   
}