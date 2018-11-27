#include "progress_bar.h"

char bar_style = '*';
char bar_valid[] = "+=-#*$&@!|08ZIH";

void progress_draw(int percent, int flag, float seconds)
{
    static char buff[53];
    static char bar_init = ' ';
    int num = 0;
    static int cnt = 0;

    cnt ++;
    if(percent > 100)
        percent = 100;
    num = percent/2;

    if(flag == BAR_ADD)
    {
        for(int i = 1; i <= num; i++)
        {
            buff[i] = bar_style;
        }
        printf("      %s [%3d%%][%5.1fs]\r",buff,percent,seconds);
    }
    else if(flag == BAR_INIT)
    {
        memset(buff,bar_init,sizeof(buff));
        buff[0] = '[';
        buff[51] = ']';
        buff[52] = '\0';

        for(int i = 1; i <= num; i++)
        {
            buff[i] = bar_style;
        }

        printf("      %s [%3d%%][%5.1fs]\r",buff,percent,seconds);
    }
    else if(flag == BAR_END)
    {
        memset(buff,bar_style,sizeof(buff));
        buff[0] = '[';
        buff[51] = ']';
        buff[52] = '\0';
        printf("      %s [%3d%%][%5.1fs]\n",buff,100,seconds);
    }
}
