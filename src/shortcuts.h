#ifndef SHORTCUTS_H
#define SHORTCUTS_H

void init_shortcuts();
int buffer_has_shortcut(struct block* buffer, int curmode);
struct shortcut* buffer_get_shortcut(struct block* buffer, int curmode);

#endif