#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


int main()
{
        time_t t;
        struct tm *pt;
        char format[128];
        char format2[128];
        int ll;

        time(&t);
        strftime(format,128,"%Y%M%d_%H%M%S\n",localtime(&t)); //년월일_시분초
        strftime(format2,128,"%Y%M%d_%H\n",localtime(&t)); //년월일_시분초
        printf("%s\n", format);
        printf("%s\n", format2);


}
