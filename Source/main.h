#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

///-----------------include----------------------
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <dir.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <winver.h>

#include "discription.h"
#include "chose_file_handle.h"
#include "dir_creat.h"
#include "file_list_get.h"
#include "set_inf_name.h"

///-----------------define----------------------
#define     FLAG_FAULT      1
#define     FLAG_OK         2
#define     PATH_LENGTH     250
#define     BUFFER_LENGTH   200
#define     MAX_BUFFER      5000
#define     LINE_LENGTH     500
#define     SIGNAL_LENGTH   100
#define     SIGNAL_NUM      800
#define     ID_LENGTH       20
#define     ID_NUM          100
#define     X4_ID_NUM       400
#define     NORMAL_LENGTH   8
#define     TMP_LENGTH      500
#define     TIMEUNIT_LENGTH 5
//#define     DEBUG_SWITCH


///------------------ struct -------------------
typedef struct {
    char inf_name[MAX_PATH];
    char asc_name[MAX_PATH];
}FILE_NUM_TYPEDEF;

FILE_NUM_TYPEDEF chose_info[MAX_PATH];

typedef struct{
    char signal_id[ID_LENGTH];
    char signal_name[SIGNAL_LENGTH];
    int left_shift;
    int right_shift;
    float precision;
    short decimals;     ///分辨率的小数位数
    signed int offset;
    int  id_order;
}CONVERT_INFO_TYPEDEF;

CONVERT_INFO_TYPEDEF convert_info[SIGNAL_NUM];

typedef enum
{
    unit_s = 0,
    unit_min,
    unit_h
}TIME_UNIT;


///-----------------extern---------------------
extern int  id_order_in_buffer[MAX_PATH];
extern char INF_PATH[MAX_PATH];
extern char DBC_PATH[MAX_PATH];
extern char inf_to_use[MAX_PATH];
extern char asc_to_use[MAX_PATH];
extern char csv_name[MAX_PATH];
extern char path_of_use_dbc[MAX_PATH];
extern char signals_id_buffer[X4_ID_NUM];
extern char signals_name_buffer[MAX_BUFFER];
extern int  num_of_inf;
extern int  num_of_asc;
extern int  num_of_signals;
extern float convert_cycle;
extern int  convert_num;
extern int  num_of_id;
extern short convert_all;
extern short join_export;
extern short show_file_name;
extern int  time_unit;

//extern short VERSION_FIRST;
//extern short VERSION_SECOND;
//extern short VERSION_THIRD;
//extern short VERSION_LAST;

extern FILE *FILE_creat_csv_file;

///----------functions declaration--------------
void creat_directory (void);
void creat_dbc_index (char *dbc_name);
void software_discription (void);
void creat_dbc_index (char *dbc_name);
void check_dbc_and_creat_inf (void);
void creat_inf_head(FILE *inf_name,char *dbc_path);
void parameter_ini(void);
void get_inf_list (void);
void get_asc_list (void);
void chose_inf_to_convert(void);
void chose_asc_to_convert(void);
void analysis_inf_and_dbc (char *inf_name);
void convert_asc (char *asc_name, long long file_size);
void get_csv_name (char *asc_name);
int get_decimals(char *decimal_str);
#endif // MAIN_H_INCLUDED
