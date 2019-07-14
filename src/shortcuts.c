#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "parser.h"

static int initted = 0;
struct dictionary * short_dict;
struct shortcut_table* table;

int buffer_has_shortcut(struct block* buffer, int curmode)
{
    struct shortcut_list* shortcut_list = NULL;
    if( curmode == VISUAL_MODE )
    {
        shortcut_list = table->visual_mode_shortcut_list;
    }
    else if( curmode == NORMAL_MODE )
    {
        shortcut_list = table->normal_mode_shortcut_list;
    }
    else if( curmode == EDIT_MODE )
    {
        shortcut_list = table->edit_mode_shortcut_list;
    }
    else if( curmode == INSERT_MODE )
    {
        shortcut_list = table->insert_mode_shortcut_list;
    }

    if( !shortcut_list || !shortcut_list->shortcut) return 0;

    struct shortcut_list* temp_list = shortcut_list;

    int res = 0;

    while(temp_list)
    {
        int tmp_res = compare_buffer(buffer, temp_list->shortcut->values);
        if(tmp_res == 1)
        {
            return 1;
        }
        else if(tmp_res == -1)
        {
            res = -1;
        }

        temp_list = temp_list->next;
    }

    return res;
}

struct shortcut* buffer_get_shortcut(struct block* buffer, int curmode)
{
    struct shortcut_list* shortcut_list = NULL;
    if( curmode == VISUAL_MODE )
    {
        shortcut_list = table->visual_mode_shortcut_list;
    }
    else if( curmode == NORMAL_MODE )
    {
        shortcut_list = table->normal_mode_shortcut_list;
    }
    else if( curmode == EDIT_MODE )
    {
        shortcut_list = table->edit_mode_shortcut_list;
    }
    else if( curmode == INSERT_MODE )
    {
        shortcut_list = table->insert_mode_shortcut_list;
    }

    if( !shortcut_list || !shortcut_list->shortcut) return NULL;

    struct shortcut_list* temp_list = shortcut_list;

    struct shortcut *shortcut = NULL;
    
    while(temp_list)
    {
        int res = compare_buffer(buffer, temp_list->shortcut->values) ;
        if( res == 1 )
        {
            return temp_list->shortcut;
        }
        else if( res == -1)
        {
            shortcut = temp_list->shortcut;
        }

        temp_list = temp_list->next;
    }

    return shortcut;
}


void init_shortcuts()
{
    if( !initted )
    {
        table = parse_shortcut_file();

        struct shortcut_list* normal_shortcut_list = table->normal_mode_shortcut_list;
        
        if(!normal_shortcut_list || !normal_shortcut_list->shortcut)
        {
            fputs("not fund any shortcuts\n", stderr);
        }
        else
        {
            struct shortcut_list* temp_list = normal_shortcut_list;

            while(temp_list)
            {
                puts(temp_list->shortcut->key);
                temp_list = temp_list->next;
            }
        }
        // else
        // {
        //     struct nlist *list = short_dict->list;

        //     while( list )
        //     {
        //         printf("a %s: %s\n", list->key, list->val);
        //         list = list->next;
        //     }
        // }
        
        initted = 1; 
    }
}