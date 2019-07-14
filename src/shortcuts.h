#ifndef SHORTCUTS_H
#define SHORTCUTS_H

int get_test_command();
int get_shift_test_command();
void init_shortcuts();
int buffer_has_shortcut(struct block* buffer);
struct shortcut* buffer_get_shortcut(struct block* buffer);

#endif