#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "parser.h"

static int initted = 0;
struct dictionary * short_dict;

void init_shortcuts()
{
    if( !initted )
    {
        short_dict = parse_shortcut_file();
        
        if(!short_dict || !short_dict->list)
        {
            fputs(stderr, "not fund any shortcuts");
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