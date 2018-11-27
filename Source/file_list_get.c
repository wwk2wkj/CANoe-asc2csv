#include "file_list_get.h"

/*******************************************************************************
*** 功能：遍历目录下指定后缀文件，并返回文件个数
*** 参数：需要传入遍历路径，格式为："D:\\Path\\test\\"
*** 结果：遍历的文件信息存储在 struct _finddatai64_t 类型的数组中
***
*******************************************************************************/
int file_list_get (const char *path,const char *file_format,\
                   struct _finddatai64_t *temp, const int MAX_NUM)
{
    int file_num = 0;
    long Handle = 0;
    char file_path[MAX_PATH];

    strcpy(file_path, path);
    strcat(file_path, file_format);

    if((Handle = _findfirsti64(file_path, &temp[file_num++])) != -1L)
        while(_findnexti64(Handle, &temp[file_num++]) != -1L &&\
              file_num <= MAX_NUM);

    _findclose(Handle);
    return file_num-1;
}
