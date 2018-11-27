#include "main.h"

#define MAX_INF_NUM   30
#define MAX_ASC_NUM   99

char INF_PATH[MAX_PATH] = "D:\\JustConvert\\" ;
char INF_FORMAT[] = "*.inf";
char DBC_PATH[MAX_PATH] = "D:\\JustConvert\\DbcFiles\\" ;
char DBC_FOAMAT[] = "*.dbc";
char ASC_FORMAT[] = "*.asc";
int  num_of_inf = 0;
int  num_of_signals = 0;
int  num_of_asc = 0;
int  convert_num = 0;
int  num_of_id = 0;
int  time_unit = 0;
short convert_all = 0;
int  id_order_in_buffer[MAX_PATH];
char inf_to_use[MAX_PATH];
char asc_to_use[MAX_PATH];
char csv_name[MAX_PATH];
char path_of_use_dbc[MAX_PATH];
char signals_id_buffer[X4_ID_NUM];
char signals_name_buffer[MAX_BUFFER];

float convert_cycle = 0.0;
short join_export = 0;
short show_file_name = 0;

FILE *FILE_creat_csv_file = NULL;

struct _finddatai64_t  inf_file[MAX_INF_NUM];
short inf_num = 0;
struct _finddatai64_t  asc_file[MAX_ASC_NUM];
short asc_num = 0;

int main()
{
    float cost_time = 0;

    memset(inf_to_use,0,sizeof(inf_to_use));
    memset(asc_to_use,0,sizeof(asc_to_use));
    memset(signals_name_buffer,0,sizeof(signals_name_buffer));
    signals_name_buffer[0] = ',';
    memset(path_of_use_dbc,0,sizeof(path_of_use_dbc));

    software_discription();
        if(!good_day)
            printf("                      <初始化>\n");
        else
            printf("                      <GOOD DAY>\n");
        printf(" -> NO.1......\n");
    creat_dbc_dir();
        printf(" -> NO.2......\n");
    check_dbc_and_creat_inf ();
        printf(" -> NO.3......\n");
    inf_num = file_list_get(INF_PATH,INF_FORMAT,inf_file,MAX_INF_NUM);
    chose_file_handle(set_inf_name,inf_file,inf_num,convert_all);
        printf("    ->inf file : %s\n",inf_to_use);
        printf(" -> NO.4......\n");
    analysis_inf_and_dbc(inf_to_use);
        printf("    ->dbc file : %s\n",path_of_use_dbc);
        printf("\n                     <数据转换>\n");
        printf(" -> NO.5......\n");
    asc_num = file_list_get(".\\",ASC_FORMAT,asc_file,MAX_ASC_NUM);
    chose_file_handle(convert_asc,asc_file,asc_num,convert_all);

    if(!good_day)
        printf("   ->处理完成！\n");
    else
        printf("   ->GOOD DAY！\n");
    cost_time = (float)(clock() - clock_start)/CLOCKS_PER_SEC;//---计算转换耗时
    printf("   ->转换用时约: %.2f 秒\n",cost_time);

    if(join_export)
    {
        fclose(FILE_creat_csv_file);
    }
    system("pause");
    return 0;
}
