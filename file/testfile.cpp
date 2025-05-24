// #include <stdio.h>
// #include<string.h>
// int main()
// {
//     FILE *fp;
//     char str[] = "linux so easy!";

//     fp = fopen("bite", "w+");
//     fwrite(str, strlen(str), 1, fp);
//     char out[16];
//     fseek(fp, 0, SEEK_SET);
//     fread(out,strlen(str)+1,1,fp);
//     printf("%s\n",out);
//     fclose(fp);
// }
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<stdio.h>
// int main()
// {
//     int fd=open("bite",O_CREAT|O_RDWR);
//     char buf[]="i like linux!";
//     write(fd,buf,strlen(buf));
//     char out[32];
//     lseek(fd,0,SEEK_SET);
//     read(fd,out,strlen(buf));
//     printf("%s\n",out);
//     close(fd);
// }
int main()
{
    execlp("ls","-a","-l",NULL);
}
