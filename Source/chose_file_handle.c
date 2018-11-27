#include "chose_file_handle.h"
#include "console_color.h"
#include "discription.h"

#define BYTE_KB         1024
#define MB_THRS         1024000
#define BYTE_MB         1048576
#define GB_THRS         1048576000
#define BYTE_GB         1073741824

#define UNIT_KB         0
#define UNIT_MB         1
#define UNIT_GB         2
#define NAME_LEN        55

unsigned long clock_start = 0;//------------用于计算转换耗时

void name_print(char *name_buff)
{
    printf("%s ",name_buff);
}

void unit_print(float file_size, char file_unit)
{
    printf("[%6.2f %cB]\n",file_size,file_unit);
}

void format_print(char *file_name, long long file_size)
{
    static char  size_unit[] = "KMG";
    static char  fill = '-';
    static char  zh_fill = '~';
    short zh_half = 0;
    char  name_buff[NAME_LEN];
    short unit_index = UNIT_KB;
    int   name_len = 0;
    float real_size = 0.0;

    memset(name_buff,0,sizeof(name_buff));

    if(file_size >= GB_THRS)
    {
        unit_index = UNIT_GB;
        real_size = (float)(file_size*100/BYTE_GB)/100;
    }
    else if(file_size >= MB_THRS)
    {
        unit_index = UNIT_MB;
        real_size = (float)(file_size*100/BYTE_MB)/100;
    }
    else
    {
        unit_index = UNIT_KB;
        real_size = (float)(file_size*100/BYTE_KB)/100;
    }

    name_len = strlen(file_name);

    if(name_len < NAME_LEN)
    {
        strcpy(name_buff,file_name);
        for(int i = name_len; i < NAME_LEN-1; i++)
            name_buff[i] = fill;
        name_buff[NAME_LEN-1] = '\0';
    }
    else
    {
        if(file_name[NAME_LEN-2] & 0x80)
        {
            for(int i = 0; i < NAME_LEN-2; i++)
            {
                name_buff[i] = file_name[i];
                if(file_name[i] & 0x80)
                    zh_half = ~zh_half;
                else
                    zh_half = 0;
            }
            if(zh_half || (file_name[NAME_LEN-2] & 0x80) == 0)
                name_buff[NAME_LEN-2] = file_name[NAME_LEN-2];
            else
                name_buff[NAME_LEN-2] = zh_fill;
        }
        else
        {
            strncpy(name_buff,file_name,NAME_LEN-1);
        }
        name_buff[NAME_LEN-1] = '\0';
    }
    name_print(name_buff);
    unit_print(real_size,size_unit[unit_index]);
}

void chose_file_handle(void (*f)(char*,long long),struct _finddatai64_t *file,\
                       int num, int enable_all)
{
    char c_input[MAX_PATH];
    int  input_num;
    int  cnt = 0;

    if(num == 0)
    {
        printf("    ->WARNING: 找不到指定后缀文件!\n\n");
        console_color_set(0,0,1);
        system("pause");
        exit(EXIT_FAILURE);
    }
    else if(num == 1)
    {
        printf("    ->File: ");
        format_print(file[0].name,file[0].size);
        clock_start = clock();
        (*f)(file[0].name,file[0].size); //FUNCTION
    }
    else if(num > 1)
    {
        printf("   ->共找到%2d个文件，请输入对应序号选择其中一个...\n",num);
        if(enable_all)
            printf("   ->如果想要选择全部文件，请输入数字“0”...\n");

        for(int i = 0; i < num ; i++)
        {
            printf("    %2d:", i+1);
            format_print(file[i].name,file[i].size);
        }

        printf("    ->请输入相应数字并按回车键继续...\n     ");
        fgets(c_input,MAX_PATH,stdin);     //使用'scanf'可能导致溢出
        input_num = strtol(c_input,NULL,10);

        for(;;)
        {
            if(enable_all && (strcmp(c_input,"0\n") == 0))
            {
                printf("   ->开始处理列出的所有文件...\n");
                clock_start = clock();
                for(int j = 0; j < num; j++)
                {
                    printf("    ->正在处理第%2d个: %s\n",j+1,file[j].name);
                    printf("    ->剩余文件数：%2d...\n",num-j-1);
                    (*f)(file[j].name,file[j].size);
                }
                break;
            }
            else if((input_num >= 1)&&(input_num <= num))
            {
                printf("    ->已选择文件：%s\n",file[input_num-1].name);
                clock_start = clock();
                (*f)(file[input_num-1].name,file[input_num-1].size);   //FUNCTION
                break;
            }
            else
            {
                if(cnt ++ == 1)
                {
                    cnt = 0;
                    console_color_set(0,0,1);
                }
                printf("   ->共找到%2d个文件，请输入对应序号选择其中一个...\n",num);
                if(enable_all)
                    printf("   ->如果想要选择全部文件，请输入数字“0”...\n");

                for(int i = 0; i < num ; i++)
                {
                    printf("    %2d:", i+1);
                    format_print(file[i].name,file[i].size);
                }

                printf("    ->输入错误！请输入%d～%d范围内的数字...\n     ",enable_all?0:1,num);
                fgets(c_input,MAX_PATH,stdin);
                input_num = strtol(c_input,NULL,10);
            }
        }
    }
}
