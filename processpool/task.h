#pragma once
#include<iostream>
using namespace std;
#define taskNum 4
typedef void(*task_t)();
task_t task[taskNum];
void print()
{
    cout << "print task" << endl;
}
void Download()
{
    cout << "download task" << endl;
}
void Upload()
{
    cout << "upload task" << endl;
}
void Process()
{
    cout << "process task" << endl;
}