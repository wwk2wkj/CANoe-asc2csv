#include "main.h"
#include "progress_bar.h"

/**********************************************************************
*
* get_decimals     :计算小数对应字符串的小数有效位数
* 输入               :
*
**********************************************************************/
int get_decimals(char *decimal_str)
{
    char p_str[MAX_PATH];
    memset(p_str,0,sizeof(p_str));
    int len = 0;
    int num = 0;

    strcpy(p_str,decimal_str);

    len = strlen(p_str);
    num = len;
    for(int i=0; i<len; i++)
    {
        if(p_str[i] != '.')
            num -= 1;
        else
            break;
    }
    for(int j=0; j<len; j++)
    {
        if((p_str[len-j-1] == '0')||(p_str[len-j] == '\0')||(p_str[len-j] == '\n'))
            num -= 1;
        else
            break;
    }
    num -= 1;
    return num;
}
/**********************************************************************
*
*
**********************************************************************/
void analysis_inf(char *inf_name,char *dbc_path,char *signals_tmp)
{
    FILE *FILE_read_inf = NULL;
    char *inf_read_status = NULL;
    char dbc_read_buffer[BUFFER_LENGTH];
    char inf_read_buffer[SIGNAL_LENGTH];
    memset(dbc_read_buffer,0,sizeof(dbc_read_buffer));
    memset(inf_read_buffer,0,sizeof(inf_read_buffer));

//    char signals_name_buffer_tmp[MAX_BUFFER];
//    memset(signals_name_buffer_tmp,0,sizeof(signals_name_buffer_tmp));
//    signals_name_buffer_tmp[0] = ',';

    int  num = 0;
    char *p;
    char tmp[TMP_LENGTH];
    char cycle[MAX_PATH];
    char unit_tmp[TIMEUNIT_LENGTH];
    char name_tmp[SIGNAL_LENGTH];
    memset(tmp,0,sizeof(tmp));
    memset(cycle,0,sizeof(cycle));
    memset(name_tmp,0,sizeof(name_tmp));

    char path_of_use_inf[MAX_PATH];
    memset(path_of_use_inf,0,sizeof(path_of_use_inf));

    short id_i[SIGNAL_NUM];
    memset(id_i,0,sizeof(id_i));
    char  signal_name[SIGNAL_LENGTH];
    char  precision[NORMAL_LENGTH];
    char  id_buffer[ID_LENGTH];
    char  bar_tmp = ' ';
    memset(precision,0,sizeof(precision));
    memset(signal_name,0,sizeof(signal_name));
    memset(id_buffer,0,sizeof(id_buffer));
    memset(id_order_in_buffer,0,sizeof(id_order_in_buffer));

    strcpy(path_of_use_inf,INF_PATH);
    strcat(path_of_use_inf,inf_name);
    FILE_read_inf = fopen(path_of_use_inf,"r");

    if(FILE_read_inf == NULL)                      ///判断文件打开是否成功
    {
        printf("    ->警告：打开%s失败！请确认后重试！01\n",path_of_use_inf);
        system("pause");
        exit(EXIT_FAILURE);
    }

    do
    {
        memset(inf_read_buffer,0,sizeof(inf_read_buffer));
        inf_read_status = fgets(inf_read_buffer,SIGNAL_LENGTH,FILE_read_inf);

        if(inf_read_buffer[0] == '#')
        {
            if((p = strstr(inf_read_buffer,"#DBC_PATH :")))   //获取dbc路径
            {
                strcpy(dbc_path,p+12);
                num = strlen(dbc_path) - 1;
                dbc_path[num] = '\0';
                num = 0;
                p = 0;
            }
            else if((p = strstr(inf_read_buffer,"#time:")))   //获取转换周期
            {
                strcpy(cycle,p+6);
                convert_cycle = strtof(cycle,NULL);
                p = 0;
            }
            else if ((p = strstr(inf_read_buffer,"#TimeUnit:")))  //获取时间显示单位
            {
                strcpy(unit_tmp,p+10);
                time_unit = (unit_tmp[0] == 'h') ? unit_h : ((unit_tmp[0] == 'm') ? unit_min : unit_s);
                p = 0;
            }
            else if((p = strstr(inf_read_buffer,"#ConvertAll:")))   //是否使能全选
            {
                strcpy(tmp,p+12);
                convert_all = (strtod(tmp,NULL) == 1) ? 1 : 0;
                p = 0;
            }
            else if((p = strstr(inf_read_buffer,"#JoinExport:")))   //是否合并输出
            {
                strcpy(tmp,p+12);
                join_export = (strtod(tmp,NULL) == 1) ? 1 : 0;
                p = 0;
            }
            else if((p = strstr(inf_read_buffer,"#ShowFileName:")))   //是否合并输出
            {
                strcpy(tmp,p+14);
                show_file_name = (strtod(tmp,NULL) == 1) ? 1 : 0;
                p = 0;
            }
            else if((p = strstr(inf_read_buffer,"#BarStyle:")))   //进度条样式
            {
                bar_tmp = *(p+10);
                for(int i = 0; bar_valid[i] != '\0'; i++)
                    if(bar_tmp == bar_valid[i])
                    {
                        bar_style = bar_tmp;
                        break;
                    }
                p = 0;
            }
            else                    //获取信号名和信号数量
            {
                strcpy(name_tmp,inf_read_buffer+1);
                num = strlen(name_tmp) - 1;
                name_tmp[num] = ',';    //防止一个信号名是另一个信号名的一部分
                name_tmp[num+1] = '\0';
                strcat(signals_tmp,name_tmp);//把所有要用的signals放进一个字符串里
            }
        }
    }
    while(inf_read_status != NULL);
    fclose(FILE_read_inf);
}

void analysis_dbc(char *signals_tmp,char *dbc_path)
{
    FILE *FILE_read_dbc = NULL;
    char *dbc_read_status = NULL;
    char dbc_read_buffer[BUFFER_LENGTH];
    char inf_read_buffer[SIGNAL_LENGTH];
    memset(dbc_read_buffer,0,sizeof(dbc_read_buffer));
    memset(inf_read_buffer,0,sizeof(inf_read_buffer));

//    char signals_name_buffer_tmp[MAX_BUFFER];
//    memset(signals_name_buffer_tmp,0,sizeof(signals_name_buffer_tmp));
//    signals_name_buffer_tmp[0] = ',';

    const short left_shift[64] = {
        7  ,6  ,5  ,4  ,3  ,2  ,1  ,0  ,
        15 ,14 ,13 ,12 ,11 ,10 ,9  ,8  ,
        23 ,22 ,21 ,20 ,19 ,18 ,17 ,16 ,
        31 ,30 ,29 ,28 ,27 ,26 ,25 ,24 ,
        39 ,38 ,37 ,36 ,35 ,34 ,33 ,32 ,
        47 ,46 ,45 ,44 ,43 ,42 ,41 ,40 ,
        55 ,54 ,53 ,52 ,51 ,50 ,49 ,48 ,
        63 ,62 ,61 ,60 ,59 ,58 ,57 ,56 };

    const short right_shift[64] = {
        56 ,57 ,58 ,59 ,60 ,61 ,62 ,63 ,
        48 ,49 ,50 ,51 ,52 ,53 ,54 ,55 ,
        40 ,41 ,42 ,43 ,44 ,45 ,46 ,47 ,
        32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,
        24 ,25 ,26 ,27 ,28 ,29 ,30 ,31 ,
        16 ,17 ,18 ,19 ,20 ,21 ,22 ,23 ,
        8  ,9  ,10 ,11 ,12 ,13 ,14 ,15 ,
        0  ,1  ,2  ,3  ,4  ,5  ,6  ,7  };

    char *p;
    char tmp[TMP_LENGTH];
    char cycle[MAX_PATH];
    char name_tmp[SIGNAL_LENGTH];
    memset(tmp,0,sizeof(tmp));
    memset(cycle,0,sizeof(cycle));
    memset(name_tmp,0,sizeof(name_tmp));

    char path_of_use_inf[MAX_PATH];
    memset(path_of_use_inf,0,sizeof(path_of_use_inf));

    short i = 0;
    short id_i[SIGNAL_NUM];
    memset(id_i,0,sizeof(id_i));
    short save_id = 0;
    int   flag = 0x00;
    unsigned long int value_tmp = 0;
    int   i_signal_start = 0;
    int   i_signal_bits = 0;
    float f_precision = 0.0;
    signed int s_offset = 0;
    char  signal_name[SIGNAL_LENGTH];
    char  precision[NORMAL_LENGTH];
    char  id_buffer[ID_LENGTH];
    memset(precision,0,sizeof(precision));
    memset(signal_name,0,sizeof(signal_name));
    memset(id_buffer,0,sizeof(id_buffer));
    memset(id_order_in_buffer,0,sizeof(id_order_in_buffer));

    FILE_read_dbc = fopen(dbc_path,"r");

    if(FILE_read_dbc == NULL)                      ///判断文件打开是否成功
    {
        printf("    ->警告：打开%s失败！请确认后重试！02\n",dbc_path);
        system("pause");
        exit(EXIT_FAILURE);
    }

    do
    {
        dbc_read_status = fgets(dbc_read_buffer,BUFFER_LENGTH,FILE_read_dbc);

        flag = 0x00;

        memset(precision,0,sizeof(precision));
        memset(signal_name,0,sizeof(signal_name));
        memset(tmp,0,sizeof(tmp));

        for(i=0; i < 3; i++)
        {
            if((dbc_read_buffer[i] == 'B')&&(dbc_read_buffer[i+1] == 'O')&&(dbc_read_buffer[i+2] == '_')&&(dbc_read_buffer[i+3] == ' '))
            {
                memset(id_buffer,0,sizeof(id_buffer));
                sscanf(dbc_read_buffer,"%*s%lu",&value_tmp);     ///ID
                sprintf(id_buffer,"%lX",value_tmp);
                if(strlen(id_buffer) > 4)                       ///去除DBC中扩展帧ID最前面添加的8
                {
                    for(int i = 0;i < strlen(id_buffer);i++)
                        id_buffer[i] = id_buffer[i+1];
                }
                save_id = 1;

                break;
            }
            else if((dbc_read_buffer[i] == 'S')&&(dbc_read_buffer[i+1] == 'G')&&(dbc_read_buffer[i+2] == '_')&&(dbc_read_buffer[i+3] == ' '))
            {
                sscanf(dbc_read_buffer,"%*s%s",signal_name);
                flag |= 0x01 << 0;
                strcpy(tmp,",");
                strcat(tmp,signal_name);
                strcat(tmp,",");        ///防止一个信号名是另一个信号名的一部分
                if(strstr(signals_tmp,tmp) == 0)       ///SIGNAL NAME
                    break;

                p = strchr(dbc_read_buffer,':');
                strcpy(tmp,p+2);
                sscanf(tmp,"%d",&i_signal_start);
                flag |= 0x01 << 1;

                p = strchr(dbc_read_buffer,'|');
                strcpy(tmp,p+1);
                sscanf(tmp,"%d",&i_signal_bits);
                flag |= 0x01 << 2;

                p = strchr(dbc_read_buffer,'(');
                strcpy(tmp,p+1);
                sscanf(tmp,"%f",&f_precision);
                flag |= 0x01 << 3;

                p = strchr(dbc_read_buffer,',');
                strcpy(tmp,p+1);
                sscanf(tmp,"%d",&s_offset);
                flag |= 0x01 << 4;
                break;
            }
        }

        if(flag == 0x1F)
        {
            if(save_id == 1)
            {
                strcat(signals_id_buffer,id_buffer);///把所有要用的ID放进一个字符串里
                strcat(signals_id_buffer,",");
                save_id = 0;
                num_of_id = num_of_id + 1;

                id_order_in_buffer[num_of_id] = strlen(id_buffer) + id_order_in_buffer[num_of_id-1] + 1;    ///按顺序将ID的位置放到数组里
            }

            strcpy(convert_info[convert_num].signal_id , id_buffer);
            strcpy(convert_info[convert_num].signal_name,signal_name);
            convert_info[convert_num].left_shift = left_shift[i_signal_start];
            convert_info[convert_num].right_shift = right_shift[i_signal_start]+1-i_signal_bits;
            convert_info[convert_num].precision = f_precision;
            convert_info[convert_num].offset = s_offset;
            convert_info[convert_num].id_order = num_of_id - 1;
            p = NULL;
            if((p = strstr(signal_name,"_HSC")))
            {
                strcpy(p,"\0\0");       ///去掉 signal name 里的 _HSC
            }
            strcat(signals_name_buffer,signal_name);///把所有要用的signals放进一个字符串里
            strcat(signals_name_buffer,",");
            num_of_signals ++;

            sprintf(precision,"%f",convert_info[convert_num].precision);
            convert_info[convert_num].decimals = get_decimals(precision);
            convert_num ++;
        }
    }
    while(dbc_read_status != NULL);
    fclose(FILE_read_dbc);
}

void analysis_inf_and_dbc (char *inf_name)
{
    char dbc_path[MAX_PATH];
    char signals_tmp[MAX_BUFFER];

    memset(signals_tmp,0,sizeof(signals_tmp));
    memset(dbc_path,0,sizeof(dbc_path));

    signals_tmp[0] = ',';

    analysis_inf(inf_name,dbc_path,signals_tmp);
    strcpy(path_of_use_dbc,dbc_path);
    analysis_dbc(signals_tmp,dbc_path);
}
