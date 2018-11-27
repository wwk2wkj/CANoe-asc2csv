#include "console_color.h"

void color_set(int v_back, int v_front)
{
    char scolor[] = "color ";
    char vcolor[] = "0123456789ABCDEF";
    char color_cmd[10];
    char color_b[2];
    char color_f[2];

    memset(color_cmd,0,sizeof(color_cmd));
    memset(color_b,0,sizeof(color_b));
    memset(color_f,0,sizeof(color_f));
    strcpy(color_cmd,scolor);
    sprintf(color_b,"%c",vcolor[v_back]);
    sprintf(color_f,"%c",vcolor[v_front]);
    strcat(color_cmd,color_b);
    strcat(color_cmd,color_f);
    system(color_cmd);
}

void console_color_set(int v_back, int v_front, int rand_enable)
{
    static int srand_once = 0;
    static int color[27][2] = {
        {0,7},{0,10},{0,14},{1,10},{1,14},{3,10},{ 3,14},
        {4,10},{4,11},{5,10},{7,9},{8,10},{8,14},{11,13},
        {9, 7},{10, 9},{10,12},{11, 4},{11, 9},{11,12},
        {12,14},{14,12},{15, 1},{15, 2},{15,9},{15,12},{15,13},
    };
    int index = 0;
    int set_back = 0;
    int set_front = 0;

    if(rand_enable)
    {
        if(!srand_once)
        {
            srand(time(NULL));
            srand_once = 1;
        }

        index = rand()%27;
        set_back = color[index][0];
        set_front = color[index][1];
    }
    else
    {
        if(v_back >= 0 && v_back <= 15 && v_front >= 0 && v_front <= 15)
        {
            set_back = v_back;
            set_front = v_front;
        }
        else
        {
            set_back = 15;
            set_front = 1;
        }
    }
    color_set(set_back,set_front);
}
