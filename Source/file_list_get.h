#ifndef FILE_LIST_GET_H_INCLUDED
#define FILE_LIST_GET_H_INCLUDED

#include <io.h>
#include <unistd.h>
#include <windows.h>

extern int file_list_get (const char *path,const char *file_format,\
                   struct _finddatai64_t *temp, const int MAX_NUM);

#endif // FILE_LIST_GET_H_INCLUDED
