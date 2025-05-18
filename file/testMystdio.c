#include "./mystdio/include/my_stdio.h"
#include<stdio.h>
int main()
{
    MyFile* fp=My_Open("test.txt","w");
    //my_fread(fp);
    const char* message="123456789";
    int cnt=10;
    char buf[128];
    while(cnt)
    {
        sprintf(buf,"%s-%d\n",message,cnt);
        my_fwrite(fp,buf,strlen(buf));
        cnt--;
        sleep(1);
        my_fflush(fp);
    }
    
    my_fclose(fp);
}