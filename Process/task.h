#pragma once
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include<iostream>
#include <sys/types.h>
#include<wait.h>
#define length 101
#define sign '#'
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
        printf("[%-100s][%c][%.1lf][current:%.1lf total:%.1lf]\r", bar, s[cnt % 4], rate,current,total);
        bar[cnt++] = sign;
        fflush(stdout);
    }
}
void DownLoad()
{
    std::cout<<"DownLoading"<<std::endl;
    //process(1024*1024*200,1024*1024);
}
void PrintLog()
{
    std::cout<<"this is printing logs"<<std::endl;
}