#include "main.h"
#include "progress_bar.h"

/**********************************************************************
*
* get_csv_name     :生成转换后的 csv 文件名
* 输入               :
*
**********************************************************************/
void get_csv_name (char *asc_name)
{
    time_t timer;
    struct tm *p;
    char p_asc_name[MAX_PATH];
    static short get_once = 0;

    memset(csv_name,0,sizeof(csv_name));

    timer = time(NULL);
    p = localtime(&timer);
    strcpy(p_asc_name,asc_name);

    strncpy(csv_name,p_asc_name,(strlen(p_asc_name)-4));
    if(!join_export)
    {
        sprintf(csv_name,"%s_%d%02d%02d.csv",csv_name,1900+(p->tm_year),1+(p->tm_mon),p->tm_mday);
    }
    else if(!get_once)
    {
        sprintf(csv_name,"%s_JoinAll_%d%02d%02d.csv",csv_name,1900+(p->tm_year),1+(p->tm_mon),p->tm_mday);
        get_once = 1;
    }
}

/**********************************************************************
*
* convert_asc     :将asc文件转换为csv文件
* 输入               :
*
**********************************************************************/
void convert_asc (char *asc_name, long long file_size)
{
    FILE *FILE_open_asc_file = NULL;
    static short open_once = 0;
    static short first_line_once = 0;
    char *open_status = NULL;
    char asc_read_buffer[BUFFER_LENGTH];
    memset(asc_read_buffer,0,sizeof(asc_read_buffer));

    unsigned long clock_start = 0;//------------用于计算转换耗时
    float cost_time = 0;
    unsigned long line_num = 0;
    short asc_id_start_flag = 0;
    short asc_id_len = 0;
    char asc_name_convert[MAX_PATH];
    char show_name[MAX_PATH];
    memset(asc_name_convert,0,sizeof(asc_name_convert));
    char *p_data = NULL;
    char *p_rx = NULL;
    char *p_id = NULL;
    short format_d_flag = 0;
    short format_ok = 0;
    long long read_size = 0;
    long long read_size_bak = 0;
    long long refresh_size = file_size>>6;
    unsigned long clock_bak = 0;
    unsigned long clock_now = 0;
    short precent_bar_new = 0;
    long refresh_cnt = CLOCKS_PER_SEC/4;
    int  n = 0;
    char tmp[TMP_LENGTH];
    char asc_id[ID_LENGTH];
    char asc_id_temp[ID_LENGTH];
    float time_stamp = 0.0;
    float time_bak = 0.0;
    char  data_buffer[BUFFER_LENGTH];
    memset(tmp,0,sizeof(tmp));
    memset(asc_id,0,sizeof(asc_id));
    memset(data_buffer,0,sizeof(data_buffer));

    char time_unit_printf[3] = {'s','m','h'};
    int  time_unit_convert[3] = {1,60,3600};
    short flag[ID_NUM];
    memset(flag,0,sizeof(flag));
    typedef struct
    {
        char data_c[26];
        char data_s[26];
        unsigned long long data_l;
    } MSG_TYPEDEF;
    static MSG_TYPEDEF msg_info[ID_NUM];

    strcpy(asc_name_convert,asc_name);
    get_csv_name(asc_name_convert);
    FILE_open_asc_file = fopen(asc_name_convert,"r");

    if(FILE_open_asc_file == NULL)
    {
        printf("打开asc文件失败，请检查后重试\n");
        exit(EXIT_FAILURE);
        system("pause");
    }

    if((join_export && !open_once) || !join_export)
    {
        open_once = 1;
        FILE_creat_csv_file = fopen(csv_name,"w");
        if(FILE_creat_csv_file == NULL)
        {
            printf("创建csv文件失败，请检查后重试\n");
            exit(EXIT_FAILURE);
            system("pause");
        }
    }

    if((join_export && !first_line_once) || !join_export)
    {
        first_line_once = 1;
        if (!show_file_name)
            fprintf(FILE_creat_csv_file,"Time_%c%s\n",time_unit_printf[time_unit],signals_name_buffer);
        else
            fprintf(FILE_creat_csv_file,"File,Time_%c%s\n",time_unit_printf[time_unit],signals_name_buffer);
    }

    clock_start = clock();
    memset(show_name,0,sizeof(show_name));
    strncpy(show_name,asc_name,strlen(asc_name)-4);

    do
    {
        memset(asc_read_buffer,0,sizeof(asc_read_buffer));
        open_status = fgets(asc_read_buffer,BUFFER_LENGTH,FILE_open_asc_file);
        read_size += strlen(asc_read_buffer);

        p_data = NULL;
        p_rx = NULL;
        p_id = NULL;
        format_d_flag = 0;
        format_ok = 0;

        p_rx = strstr(asc_read_buffer,"Rx");

        if(p_rx == NULL)
            p_rx = strstr(asc_read_buffer,"Tx");

        if(p_rx != NULL)
        {
            p_data = p_rx + 2;
            for(int i = 0; i < MAX_PATH && *(p_data+i) != '\0'; i++)
            {
                if(*(p_data+i) == 'd')
                    format_d_flag = 1;
                if(format_d_flag)
                {
                    if((*(p_data+i) <= '8' && *(p_data+i) >= '1') &&\
                        *(p_data+i+1) == ' ' &&\
                       (*(p_data+i-1) == ' ' || *(p_data+i-1) == 'd'))
                    {
                       format_ok = 1;
                       p_data += (i+2);
                       break;
                    }
                }
            }
        }

        if(format_ok)
        {
            memset(asc_id,0,sizeof(asc_id));
            memset(asc_id_temp,0,sizeof(asc_id_temp));
            sscanf(asc_read_buffer,"%f%*d%s",&time_stamp,asc_id_temp);
            asc_id_start_flag = 0;
            asc_id_len = strlen(asc_id_temp);

            if(asc_id_len >= 1)
            {
                for(int i = 0,j = 0;i < asc_id_len; i++)
                {
                    if(!asc_id_start_flag && asc_id_temp[i]>'0')
                    {
                        asc_id_start_flag = 1;
                    }
                    if(asc_id_start_flag)
                    {
                        if(i == asc_id_len-1 && (asc_id_temp[i] == 'x' || asc_id_temp[i] == 'X'))
                        {
                            break;
                        }
                        else
                        {
                            asc_id[j++] = asc_id_temp[i];
                        }
                    }
                }
            }

            strcat(asc_id,",");     //添加逗号，防止该id是其他id的一部分时获取数据出错
            p_id = strstr(signals_id_buffer,asc_id);

            if(p_id != NULL)
            {
                for(int i = 0; i < num_of_id; i++)//计算该行数据的ID是signals_id_buffer的第几个
                {
                    if(id_order_in_buffer[i] == (p_id - signals_id_buffer))
                        n = i;
                    else;
                }

                strcpy(msg_info[n].data_c,p_data);

                flag[n] = 1;

                if((time_bak > time_stamp)||(time_stamp > (time_bak + convert_cycle * 100)))
                    //判断数据是否重新开始或时间跨度过大，避免跨监控取数据
                {
                    time_bak = time_stamp;
                    memset(flag,0,sizeof(flag));
                }

                if(time_stamp - time_bak >= convert_cycle)
                {
                    if (!show_file_name)
                        fprintf(FILE_creat_csv_file,"%.3f,",time_stamp/time_unit_convert[time_unit]);
                    else
                        fprintf(FILE_creat_csv_file,"%s,%.3f,",show_name,time_stamp/time_unit_convert[time_unit]);
                    for(int j=0; j<num_of_id; j++)
                    {
                        if(flag[j])//去除数据里的空格，并转换为十六进制
                        {
                            for(int i = 0; i < 16 ; i++)
                                msg_info[j].data_s[i] = msg_info[j].data_c[i+i/2];
                            msg_info[j].data_l = strtoull(msg_info[j].data_s,NULL,16);
                        }
                    }

                    for(int j=0; j<num_of_signals; j++)
                    {
                        n = convert_info[j].id_order;
                        if(flag[n])
                        {
                            if(convert_info[j].precision == 1.0)
                                fprintf(FILE_creat_csv_file,"%d,",\
                                        ((msg_info[n].data_l << convert_info[j].left_shift)>>\
                                         (convert_info[j].left_shift+convert_info[j].right_shift))+convert_info[j].offset);
                            else if(convert_info[j].decimals == 1)
                                fprintf(FILE_creat_csv_file,"%.1f,",\
                                        convert_info[j].precision*((msg_info[n].data_l << convert_info[j].left_shift)>>\
                                                                   (convert_info[j].left_shift+convert_info[j].right_shift))+convert_info[j].offset);
                            else if(convert_info[j].decimals == 2)
                                fprintf(FILE_creat_csv_file,"%.2f,",\
                                        convert_info[j].precision*((msg_info[n].data_l << convert_info[j].left_shift)>>\
                                                                   (convert_info[j].left_shift+convert_info[j].right_shift))+convert_info[j].offset);
                            else if(convert_info[j].decimals == 3)
                                fprintf(FILE_creat_csv_file,"%.3f,",\
                                        convert_info[j].precision*((msg_info[n].data_l << convert_info[j].left_shift)>>\
                                                                   (convert_info[j].left_shift+convert_info[j].right_shift))+convert_info[j].offset);
                            else if(convert_info[j].decimals == 4)
                                fprintf(FILE_creat_csv_file,"%.4f,",\
                                        convert_info[j].precision*((msg_info[n].data_l << convert_info[j].left_shift)>>\
                                                                   (convert_info[j].left_shift+convert_info[j].right_shift))+convert_info[j].offset);
                            else if(convert_info[j].decimals == 5)
                                fprintf(FILE_creat_csv_file,"%.5f,",\
                                        convert_info[j].precision*((msg_info[n].data_l << convert_info[j].left_shift)>>\
                                                                   (convert_info[j].left_shift+convert_info[j].right_shift))+convert_info[j].offset);
                            else
                                fprintf(FILE_creat_csv_file,"%f,",\
                                        convert_info[j].precision*((msg_info[n].data_l << convert_info[j].left_shift)>>\
                                                                   (convert_info[j].left_shift+convert_info[j].right_shift))+convert_info[j].offset);
                        }
                        else
                            fprintf(FILE_creat_csv_file,",");
                    }
                    fprintf(FILE_creat_csv_file,"\n");

                    line_num ++;
                    n = 0;
                    memset(flag,0,sizeof(flag));
                    time_bak = time_stamp;
                }
            }
        }

        clock_now = clock();
        if(clock_bak + refresh_cnt <= clock_now ||\
           read_size_bak + refresh_size <= read_size)
        {
            if(precent_bar_new)
            {
                cost_time = (float)((clock_now - clock_start)*100/CLOCKS_PER_SEC)/100;
                progress_draw(read_size*100/file_size,BAR_ADD,cost_time);
            }
            else
            {
                progress_draw(read_size*100/file_size,BAR_INIT,0);
                precent_bar_new = 1;
            }
            clock_bak = clock_now;
            read_size_bak = read_size;
        }
    }
    while(open_status != NULL);
    cost_time = (float)((clock_now - clock_start)*100/CLOCKS_PER_SEC)/100;
    progress_draw(100,BAR_END,cost_time);

    fclose(FILE_open_asc_file);
    if(!join_export)
    {
        fclose(FILE_creat_csv_file);
    }
}

