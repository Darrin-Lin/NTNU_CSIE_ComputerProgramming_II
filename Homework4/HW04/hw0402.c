#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>

#ifdef DEBUG
#define debug_print(fmt, ...)                     \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
            __LINE__, __func__, ##__VA_ARGS__);
#else
#define debug_print(fmt, ...) \
    {                         \
    }
#endif

#define HELP_MSG "\
Usage: hw0402\n\
可以選擇輸入 (N) 地點名稱（中英文皆可） 或 (U) YouBike 站代號來查詢即時資訊。\n\
顯示即時資訊的項目包括：\n\
1. 站點代號\n\
2. 中文名稱\n\
3. 英文名稱\n\
4. 更新時間\n\
5. 可租借腳踏車數量\n\
6. 可停放車柱數量\n\
"

int main(int argc, char *argv[])
{

    struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};
    int32_t opt = 0;
    while ((opt = getopt_long(argc, argv, "h", longopts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'h':
            fprintf(stdout, HELP_MSG);
            return 0;
        default:
            fprintf(stdout, HELP_MSG);
            return 0;
        }
    }
// cat youbike_immediate.json | sed "s/}/}\n/g" | grep 公館
// cat youbike_immediate.json | sed "s/},/},\n/g" | awk 'BEGIN {FS=","}; {print $2 }' | grep 公園 | awk 'BEGIN {FS="\""}; {print $4}'
// wget -q -O - https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json | sed "s/},/},\n/g" | sed "s/YouBike2.0_//g" | awk 'BEGIN {FS=","}; {print $1 $2 $7 $12 $15 $18}' | awk 'BEGIN {FS="\""}; {if (index(tolower($12),tolower("park"))) print $4"+"$8"+"$12"+"$16"+"substr($19,2)"+"substr($21,2,length($21)-2) }' | head -n 30 | awk 'BEGIN {FS="+"}; {print "UID："$1"\n中文名稱："$2"\n英文名稱："$3"\n更新時間："$4"\n可租借腳踏車數量："$5"\n可停放車柱數量："$6"\n"}'
INPUT:
    printf("請選擇您要使用 (N) 查詢的地點名稱（中英文皆可）或 (U) YouBike 站代號：");
    char mode = 0;
    char search[1024];
    char tmp[1024];
    fgets(tmp, 1024, stdin);
    mode = tmp[0];
    if (strlen(tmp) != 2)
    {
        printf("輸入錯誤\n");
        goto INPUT;
    }
    char *tmp_num = NULL;
    switch (mode)
    {
    case 'N':
    case 'n':
        printf("請輸入地點名稱：");
        fgets(search, 1024, stdin);
        search[strlen(search) - 1] = '\0';
        strtol(search, &tmp_num, 10);
        break;
    case 'U':
    case 'u':
        printf("請輸入 YouBike 站代號：");
        fgets(search, 1024, stdin);
        search[strlen(search) - 1] = '\0';
        if (strlen(search) != 9)
        {
            printf("請輸入完整 UID（9 碼）\n");
            goto INPUT;
        }
        strtol(search, &tmp_num, 10);
        if (*tmp_num != '\0')
        {
            printf("輸入錯誤\n");
            goto INPUT;
        }
        break;
    default:
        printf("輸入錯誤\n");
        goto INPUT;
        break;
    }
    printf("搜尋中...\n\n");
    char command[1000];
    FILE *pSys = NULL;
    if (mode == 'U' || mode == 'u')
    {
        snprintf(command,1000,"wget -q -O - https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json | sed \"s/},/},\\n/g\" | sed \"s/YouBike2.0_//g\" | awk 'BEGIN {FS=\",\"}; {print $1 $2 $7 $12 $15 $18}' | awk 'BEGIN {FS=\"\\\"\"}; {print $4\"+\"$8\"+\"$12\"+\"$16\"+\"substr($19,2)\"+\"substr($21,2,length($21)-2) }' | grep -i %s | head -n 5 | awk 'BEGIN {FS=\"+\"}; {print \"UID：\"$1\"\\n中文名稱：\"$2\"\\n英文名稱：\"$3\"\\n更新時間：\"$4\"\\n可租借腳踏車數量：\"$5\"\\n可停放車柱數量：\"$6\"\\n\"}'",search);
        // snprintf(command, 1000, "wget -q -O - https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json | sed \"s/},/},\\n/g\" | sed \"s/YouBike2.0_//g\" | awk 'BEGIN {FS=\",\"}; {print $1 $2 $7 $12 $15 $18}' | awk 'BEGIN {FS=\"\\\"\"}; { if (index($4,\"%s\") ) print $4\"+\"$8\"+\"$12\"+\"$16\"+\"substr($19,2)\"+\"substr($21,2,length($21)-2) }'| head -n 30 | awk 'BEGIN {FS=\"+\"}; {if(getline) print \"UID：\"$1\"\\n中文名稱：\"$2\"\\n英文名稱：\"$3\"\\n更新時間：\"$4\"\\n可租借腳踏車數量：\"$5\"\\n可停放車柱數量：\"$6\"\\n\"'", search);
    }
    else
    {

        snprintf(command, 1000, "wget -q -O - https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json | sed \"s/},/},\\n/g\" | sed \"s/YouBike2.0_//g\" | awk 'BEGIN {FS=\",\"}; {print $1 $2 $7 $12 $15 $18}' | awk 'BEGIN {FS=\"\\\"\"}; { if (index(tolower($12),tolower(\"%s\")) || index($8,\"%s\") ) print $4\"+\"$8\"+\"$12\"+\"$16\"+\"substr($19,2)\"+\"substr($21,2,length($21)-2) }'| head -n 5 | awk 'BEGIN {FS=\"+\"}; {print \"UID：\"$1\"\\n中文名稱：\"$2\"\\n英文名稱：\"$3\"\\n更新時間：\"$4\"\\n可租借腳踏車數量：\"$5\"\\n可停放車柱數量：\"$6\"\\n\"}'", search,search);
    }
    pSys = popen(command, "r");
    if (pSys == NULL)
    {
        return -1;
    }
    if (feof(pSys))
    {
        printf("err");
    }
    int8_t count = 0;
    while (!feof(pSys))
    {
        if (count < 2)
        {
            count++;
        }
        char line[1000] = {0};
        fgets(line, 1000, pSys);
        printf("%s", line);
    }
    if (count < 2)
    {
        printf("找不到資料\n");
    }
    pclose(pSys);

    return 0;
}