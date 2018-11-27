#include "main.h"

/**********************************************************************
*
* creat_dbc_index     :生成inf信号索引文件的头
* 输入               :文件指针名，dbc完整路径
*
**********************************************************************/
void creat_inf_head(FILE *inf_name,char *dbc_path)
{
    short i = 0;
    fprintf(inf_name,"/                      <文档说明>\n");
    fprintf(inf_name,"/%2d.请不要随意改动任何内容，除非你知道更改的具体后果\n",i++);
    fprintf(inf_name,"/%2d.\"/\"符号是本软件定义的屏蔽符号，以之为开头的一行不会被执行\n",i++);
    fprintf(inf_name,"/%2d.\"#\"符号是本软件定义的执行符号，以之为开头的一行将被执行\n",i++);
    fprintf(inf_name,"/%2d.如果要转换某个信号，把开头的\"/\"换成\"#\"即可\n",i++);
    fprintf(inf_name,"/%2d.time后面的数字为转换的周期，单位是“秒”\n",i++);
    fprintf(inf_name,"/%2d.转换周期建议设置为不小于0.2s\n",i++);
    fprintf(inf_name,"/%2d.TimeUnit用于设置时间显示的单位，可以是(h/min/s)\n",i++);
    fprintf(inf_name,"/%2d.JoinExport用于将所选asc文件转换到同一个csv文件中，注意文件名排序\n",i++);
    fprintf(inf_name,"/%2d.ShowFileName用于在转换结果中显示每条数据所属文件名称，1为打开该功能\n",i++);
    fprintf(inf_name,"/%2d.BarStyle用于设定进度条的样式，可用(+,=,-,#,*,$,&,@,!,|,0,8,Z,I,H)\n",i++);
    fprintf(inf_name,"\n/以下任何内容的更改都可能影响转换结果，请慎重操作！！！\n");
    fprintf(inf_name,"/（请慎重操作！！！）（请慎重操作！！！）（请慎重操作！！！）\n\n");
    fprintf(inf_name,"#DBC_PATH : %s\n", dbc_path);
    fprintf(inf_name,"#time:1.0\n");
    fprintf(inf_name,"#TimeUnit:s\n");
    fprintf(inf_name,"#ConvertAll:1\n");
    fprintf(inf_name,"#JoinExport:0\n");
    fprintf(inf_name,"#ShowFileName:0\n");
    fprintf(inf_name,"#BarStyle:*\n");
}
/**********************************************************************
*
* creat_dbc_index     :从接收的dbc文件创建 signals 索引
* 输入               :dbc文件名，不带后缀
*
**********************************************************************/
void creat_dbc_index (char *dbc_name)
{
    FILE *FILE_dbc_index_create = NULL;
    FILE *FILE_dbc_file_open = NULL;
    char *read_status = NULL;
    char read_buffer[BUFFER_LENGTH];
    char write_buffer[BUFFER_LENGTH];
    memset(read_buffer,0,sizeof(read_buffer));
    memset(write_buffer,0,sizeof(write_buffer));
    int  i = 0;
    int  BO_line = 0;
    int  SG_line_over = 0;

    char inf_path[MAX_PATH];//"D:\\JustConvert\\" ;
    char dbc_path[MAX_PATH];//"D:\\JustConvert\\DbcFiles\\" ;

    strcpy(inf_path,INF_PATH);
    strcpy(dbc_path,DBC_PATH);

    strcat(inf_path, dbc_name);
    strcat(inf_path,".inf");
    strcat(dbc_path, dbc_name);
    strcat(dbc_path,".dbc");

    FILE_dbc_index_create = fopen(inf_path , "w");
    FILE_dbc_file_open = fopen(dbc_path, "r");

    if(FILE_dbc_index_create == NULL)
    {
        printf("    ->警告：创建%s失败！请确认后重试！\n",inf_path);
        system("pause");
        exit(EXIT_FAILURE);
    }
    if(FILE_dbc_file_open == NULL)                      ///判断文件打开是否成功
    {
        printf("    ->警告：打开%s失败！请确认后重试！\n",dbc_path);
        system("pause");
        exit(EXIT_FAILURE);
    }

    creat_inf_head(FILE_dbc_index_create, dbc_path); ///写入inf信号索引文件的头

/*************** main function *******************/
    do
    {
        BO_line = 0;
        SG_line_over = 0;
        memset(write_buffer,0,sizeof(write_buffer));

        read_status = fgets(read_buffer, BUFFER_LENGTH, FILE_dbc_file_open);

        for(i=0; i < 3; i++)
        {
            if((read_buffer[i] == 'B')&&(read_buffer[i+1] == 'O')&&(read_buffer[i+2] == '_')&&(read_buffer[i+3] == ' '))
            {
                BO_line = 1;
                break;
            }
            else if((read_buffer[i] == 'S')&&(read_buffer[i+1] == 'G')&&(read_buffer[i+2] == '_')&&(read_buffer[i+3] == ' '))
            {
                sscanf(read_buffer,"%*s%s",write_buffer);//%*s 忽略第一个字符串，%s 取第二个字符串即是信号名，效果同上面被注释掉的
                SG_line_over = 1;
                break;
            }
        }

        if(BO_line)
        {
            fprintf(FILE_dbc_index_create,"\n");    //遇message行不输出，直接换行
        }
        else if(SG_line_over)
        {
            fprintf(FILE_dbc_index_create,"/");
            fprintf(FILE_dbc_index_create,write_buffer);    //遇signal行输出信号名，前面加/直接屏蔽掉
            fprintf(FILE_dbc_index_create,"\n");
        }
    }
    while(read_status != NULL);         //判断dbc文件是否读取到最后一行

    fclose(FILE_dbc_index_create);
    fclose(FILE_dbc_file_open);
    FILE_dbc_index_create = NULL;
    FILE_dbc_file_open = NULL;
}

/**********************************************************************
*
* check_dbc_and_creat_inf     :检查是否有dbc需要创建信号索引
* 输入               :
*
**********************************************************************/
void check_dbc_and_creat_inf (void)
{
    int i = 0;
    //int j = 0;
    int name_size = 0;
    long Handle = 0;
    long value_handle = 0;
    struct _finddata_t FileInfo;

    char dbc_name_with_affix[MAX_PATH];
    char dbc_name_no_affix[MAX_PATH];
    char inf_name_with_affix[MAX_PATH];
    memset(dbc_name_with_affix,0,sizeof(dbc_name_with_affix));
    memset(dbc_name_no_affix,0,sizeof(dbc_name_no_affix));
    memset(inf_name_with_affix,0,sizeof(inf_name_with_affix));

    char dbc_path[MAX_PATH];// = "D:\\JustConvert\\DbcFiles\\*.dbc" ;
    strcpy(dbc_path,DBC_PATH);
    strcat(dbc_path,"*.dbc");

    value_handle = Handle = _findfirst(dbc_path, &FileInfo);    ///查找dbc文件

    if(value_handle == -1L)
    {
        printf("    警告：在%s找不到dbc文件，请确认后重试！\n", DBC_PATH);
        system("pause");
        exit(EXIT_FAILURE);
    }

    while(value_handle != -1L)
    {
        memset(dbc_name_with_affix,0,sizeof(dbc_name_with_affix));
        memset(dbc_name_no_affix,0,sizeof(dbc_name_no_affix));
        memset(inf_name_with_affix,0,sizeof(inf_name_with_affix));
        name_size = 0;                                              ///清空，以开始下一轮

        strcpy(dbc_name_with_affix, FileInfo.name); ///获取dbc文件名

        for(i=0; ((i<MAX_PATH)&&(name_size == 0)); i++)
        {
            if(dbc_name_with_affix[i+4] == '\0')
            {
                name_size = i;
                break;
            }
        }
        memmove(dbc_name_no_affix,dbc_name_with_affix,name_size); ///去掉dbc文件名的后缀 .dbc
        strcpy(inf_name_with_affix, INF_PATH);
        strcat(inf_name_with_affix, dbc_name_no_affix);
        strcat(inf_name_with_affix, ".inf");            ///得到inf文件的完整路径，包括文件名及后缀

        if(access(inf_name_with_affix,0) == -1L)        ///判断是否已存在指定inf文件
        {
            creat_dbc_index(dbc_name_no_affix);         ///不存在情况下调用函数生成inf文件
        }
        value_handle = _findnext(Handle, &FileInfo);    ///查找下一个dbc文件
    }
    _findclose(Handle);
}



