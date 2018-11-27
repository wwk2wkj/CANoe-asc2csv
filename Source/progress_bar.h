#ifndef PROGRESS_BAR_H_INCLUDED
#define PROGRESS_BAR_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <windows.h>

#define BAR_INIT    0
#define BAR_ADD     1
#define BAR_END     2

extern char bar_style;
extern char bar_valid[];

extern void progress_draw(int percent,int new_flag, float seconds);

#endif // PROGRESS_BAR_H_INCLUDED
