#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define length 101
#define sign '#'
// V1
//  void process()
//  {
//      char bar[length];
//      memset(bar,'\0',sizeof(bar));
//      int cnt=0;
//      char s[4]={'-','/','\\','|'};
//      while(cnt<=100)
//      {
//          printf("[%-100s][%c][%d%]\r",bar,s[cnt%4],cnt);
//          bar[cnt++]=sign;
//          fflush(stdout);
//          usleep(100000);
//      }
//      printf("\n");
//  }
// V2
void process(double total, double current)
{
    char bar[length];
    int cnt = 0;
    memset(bar, '\0', sizeof(bar));
    double rate = (current * 100) / total;
    int loop_cnt=(int)rate;
    char s[4] = {'-', '/', '\\', '|'};
    while (cnt <= loop_cnt)
    {
        printf("[%-100s][%c][%.1lf%][current:%.1lf total:%.1lf]\r", bar, s[cnt % 4], rate,current,total);
        bar[cnt++] = sign;
        fflush(stdout);
    }
}