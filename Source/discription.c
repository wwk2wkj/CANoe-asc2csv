//#include "main.h"
#include "discription.h"
#include "console_color.h"

/*********************************************************************
*
* software_discription:  软件的简单介绍和声明
*
**********************************************************************/
int good_day = 0;

void version_discripton(void)
{
    time_t t = time(NULL);
    struct tm *d = localtime(&t);

    if(d->tm_mday == 16)
    {
        console_color_set(0,0,1);
        good_day = 1;
    }
    else
        system("color F1");

    printf(" Version:%d.%d.%d.%d   Author:王文科   E-mail:wwkwkj@qq.com\n",\
           VERSION_FIRST,VERSION_SECOND,VERSION_THIRD,VERSION_LAST);
    printf(" Compiled on %s at %s\n\n",__DATE__,__TIME__);
}

void other_discripton(void)
{
    short i = 0;
    printf("                      <重要声明>\n");
    printf(" %2d. 请先阅读自述文档：README.TXT\n",i++);
    printf(" %2d. 转换软件需要dbc文件才可以正常工作\n",i++);
    printf(" %2d. 请自行获取和使用dbc文件\n",i++);
    printf(" %2d. 软件会生成一份与dbc文件同名的信号索引\n",i++);
    printf(" %2d. dbc文件更新后请删除同名索引文件以重新生成\n",i++);
    printf(" %2d. 不对转换出数据做任何保证\n",i++);
    printf(" %2d. 如有疑问请先阅读自述文档：README.TXT\n",i++);
    printf(" %2d. 警告：转换出的数据可能有误!!!\n",i++);
    printf(" ....<声明结束>\n");
}

void software_discription (void)
{
    version_discripton();
    other_discripton();
}
