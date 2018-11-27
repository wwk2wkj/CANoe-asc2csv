#ifndef CHOSE_FILE_HANDLE_H_INCLUDED
#define CHOSE_FILE_HANDLE_H_INCLUDED

#include <stdio.h>
#include <unistd.h>
#include <windows.h>

extern unsigned long clock_start;

extern void chose_file_handle(void (*f)(char*,long long),struct _finddatai64_t *file,\
                       int num, int enable_all);

#endif // CHOSE_FILE_HANDLE_H_INCLUDED
