#include "dir_creat.h"

/*******************************************************************************
*** 功能：遍历目录下指定后缀文件，并返回文件个数
*** 参数：path:"D:" dir:"D:\\JustConvert"; path:"D:\\JustConvert" dir:"D:\\JustConvert\\DbcFiles"
*** 结果：遍历的文件信息存储在 struct _finddatai64_t 类型的数组中
***
*******************************************************************************/

void creat_directory (const char *path, const char *dir)
{
    if(access(dir,2))  ///return -1 : not exist , 0 : exist
    {
        if(mkdir(dir))  ///return -1 : fail , 0 : success
        {
            printf("      无法在%s创建文件夹，\n",path);
            printf("      请检查是否已存在同名文件或权限设置有误\n");
            system("pause");
            exit(EXIT_FAILURE);
        }
    }
}

void creat_dbc_dir(void)
{
    char path[] = "D:";
    char inf_path[] = "D:\\JustConvert";
    char dbc_path[] = "D:\\JustConvert\\DbcFiles";

    creat_directory(path,inf_path);
    creat_directory(inf_path,dbc_path);
}
