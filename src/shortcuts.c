#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "parser.h"

static int initted = 0;
struct dictionary * short_dict;
struct shortcut_list* slist;

int buffer_has_shortcut(struct block* buffer)
{
    if( !slist || !slist->shortcut) return 0;

    struct shortcut_list* temp_list = slist;

    while(temp_list)
    {
        if(compare_buffer(buffer, temp_list->shortcut->values))
        {
            return 1;
        }

        temp_list = temp_list->next;
    }

    return 0;
}

struct shortcut* buffer_get_shortcut(struct block* buffer)
{
    if( !slist || !slist->shortcut) return NULL;

    struct shortcut_list* temp_list = slist;

    while(temp_list)
    {
        if(compare_buffer(buffer, temp_list->shortcut->values))
        {
            return temp_list->shortcut;
        }

        temp_list = temp_list->next;
    }

    return NULL;
}


void init_shortcuts()
{
    if( !initted )
    {
        slist = parse_shortcut_file2();
        
        if(!slist || !slist->shortcut)
        {
            fputs("not fund any shortcuts\n", stderr);
        }
        else
        {
            struct shortcut_list* temp_list = slist;

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

int get_save_command()
{
    return ctl('S');
}

int get_test_command()
{
    return ctl('O');
}

int get_shift_test_command()
{
    return  0;
}