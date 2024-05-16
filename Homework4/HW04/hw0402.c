#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#define debug_print(fmt, ...)                       \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
              __LINE__, __func__, ##__VA_ARGS__);
#else
#define debug_print(fmt, ...) \
    {}
#endif

int main()
{

    // cat youbike_immediate.json | sed "s/}/}\n/g" | grep 公館
    // cat youbike_immediate.json | sed "s/},/},\n/g" | awk 'BEGIN {FS=","}; {print $2 }' | grep 公園 | awk 'BEGIN {FS="\""}; {print $4}'
    // cat youbike_immediate.json | sed "s/},/},\n/g" | sed "s/YouBike2.0_//g" | awk 'BEGIN {FS=","}; {print $1 $2 $7 $12 $15 $18}' | awk 'BEGIN {FS="\""}; {print $4"+"$8"+"$12"+"$16"+"substr($19,2)"+"substr($21,2,length($21)-2) }' | grep -i 公館 | head -n 30 | awk 'BEGIN {FS="+"}; {print "UID："$1"\n中文名稱："$2"\n英文名稱："$3"\n更新時間："$4"\n可租借腳踏車數量："$5"\n可停放車柱數量："$6"\n"}'
    char a[100]="bike";
    char command[1000];
    FILE *pSys = NULL;
    snprintf(command,1000,"wget -q -O - https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json | sed \"s/},/},\\n/g\" | sed \"s/YouBike2.0_//g\" | awk 'BEGIN {FS=\",\"}; {print $1 $2 $7 $12 $15 $18}' | awk 'BEGIN {FS=\"\\\"\"}; {print $4\"+\"$8\"+\"$12\"+\"$16\"+\"substr($19,2)\"+\"substr($21,2,length($21)-2) }' | grep -i %s | head -n 30 | awk 'BEGIN {FS=\"+\"}; {print \"UID：\"$1\"\\n中文名稱：\"$2\"\\n英文名稱：\"$3\"\\n更新時間：\"$4\"\\n可租借腳踏車數量：\"$5\"\\n可停放車柱數量：\"$6\"\\n\"}'",a);
    pSys = popen(command,"r");
    if(pSys==NULL)
    {
        return -1;
    }
    if(feof(pSys))
    {
        printf("err");
    }
    int8_t count=0;
    while(!feof(pSys))
    {
        if(count<2)
        {
            count++;
        }
        char line[1000]={0};
        fgets(line,1000,pSys);
        printf("%s",line);
    }
    if(count<2)
    {
        printf("error");
    }
    pclose(pSys);

    return 0;
}