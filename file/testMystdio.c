#include "my_stdio.h"
#include<stdio.h>
int main()
{
    MyFile* fp=My_Open("test","w");
    //my_fread(fp);
    const char* message="123456789";
    int cnt=10;
    char* buf;
    while(cnt)
    {
        sprintf(buf,"%s-%d\n",message,cnt);
        my_fwrite(fp,buf,strlen(buf));
        cnt--;
        sleep(1);
    }
    
    my_fclose(fp);
}