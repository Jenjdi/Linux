#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include<cstring>
#include "quicksort.h"
using namespace std;
void make_random_num_file(const string &path, int make_nums)
{
    srand(time(nullptr));
    FILE *fin = fopen(path.c_str(), "w");
    for (size_t i = 0; i < make_nums; i++)
    {
        int num = rand() % 10000;
        fprintf(fin, "%d\n", num);
    }
    fclose(fin);
}
void _merge_file(const char* filename1,const char* filename2,const char* tmpfilename)
{
    FILE* file1=fopen(filename1,"r");
    FILE* file2=fopen(filename2,"r");
    FILE* tmpfile=fopen(tmpfilename,"w");
    int num1,num2;
    int ret1=fscanf(file1,"%d\n",&num1);
    int ret2=fscanf(file2,"%d\n",&num2);
    while(ret1!=EOF&&ret2!=EOF)
    {
        if(num1<num2)
        {
            fprintf(tmpfile,"%d\n",num1);
            ret1=fscanf(file1,"%d\n",&num1);
        }
        else
        {
            fprintf(tmpfile,"%d\n",num1);
            ret2=fscanf(file2,"%d\n",&num1);
        }
    }
    while(ret1!=EOF)
    {
        fprintf(tmpfile,"%d\n",num1);
        ret1=fscanf(file1,"%d\n",&num1);
    }
    while(ret2!=EOF)
    {
        fprintf(tmpfile,"%d\n",num2);
        ret2=fscanf(file2,"%d\n",&num2);
    }
    fclose(file1);
    fclose(file2);
    fclose(tmpfile);
}
void filesort(const string &path, int read_once_num)
{
    FILE *fout = fopen(path.c_str(), "r");
    vector<int> arr(read_once_num);
    char file_name[32];
    int arr_ptr = 0;
    int current_saved_num;
    int file_num = 0;
    while (fscanf(fout, "%d\n", &current_saved_num) != EOF)
    {
        if (arr_ptr < read_once_num - 1)
            arr[arr_ptr++] = current_saved_num; //
        else
        {
            arr[arr_ptr] = current_saved_num;
            QuickSortN(arr, 0, read_once_num);
            sprintf(file_name, "./sorted_files/sorted_file_%d", file_num++);
            FILE *fin = fopen(file_name, "w");
            if (fin == nullptr)
            {
                cout << "打开文件失败" << endl;
                return;
            }
            for (int i = 0; i < read_once_num; i++)
            {
                fprintf(fin, "%d\n", arr[i]);
            }
            fclose(fin);
            arr_ptr = 0;

        }
        
    }
    //文件之间相互归并，实现整体有序
    char tmpfile[40]="./sorted_files/sorted_file_all";
    char filename1[40]="./sorted_files/sorted_file_0";
    char filename2[40];
    for(int i=1;i<file_num;i++)
    {
        sprintf(filename2, "./sorted_files/sorted_file_%d", i);
        _merge_file(filename1,filename2,tmpfile);
        strcpy(filename1,tmpfile);
        sprintf(tmpfile,"%s%d",tmpfile,i);
    }
    
    fclose(fout);
}
int main()
{
    //make_random_num_file("./sort_file/file.txt",10000);
    filesort("./sort_file/file.txt", 1000);
}