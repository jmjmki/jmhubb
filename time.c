#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define TIME_FILE 0
#define TIME_DIR  1

#define PATH_NAME "/home/user/coding/"


void ttime(int type, char *buf)
{
        time_t realtime;
        struct tm* local_timeinfo;
        time(&realtime);
        local_timeinfo = localtime(&realtime);

        if(type == TIME_FILE){
        //파일네임
                strftime(buf, 128, "%Y%m%d_%H%M%S", local_timeinfo);
        }
        else {
        //디렉토리 네임
                strftime(buf, 128, "%Y%m%d_%H", local_timeinfo);
        }
}
int main()
{
        char *mkdirname;
        char *mkfilename;
        char path_temp[100];
        sprintf(path_temp,"%s",  PATH_NAME);
        mkdirname = (char*) malloc(100);
        mkfilename = (char*) malloc(100);
        
        ttime(TIME_FILE, mkfilename);
        printf("%s\n", mkfilename);
        ttime(TIME_DIR, mkdirname);
        printf("%s\n", mkdirname);
        strcat(path_temp, mkdirname);
        printf("%s\n", path_temp);
}

                                                                         45,0-1       바닥
