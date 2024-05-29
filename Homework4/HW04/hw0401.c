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
#define debug_print(fmt, ...)                       \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
              __LINE__, __func__, ##__VA_ARGS__);
#else
#define debug_print(fmt, ...) \
    {}
#endif

#define HELP_MSG "\
Usage: hw0401 [options]\n\
  -t, --time-interval=time Update the information every [time] seconds. Default: 5 seconds.\n\
  -c, --count=count Update the information [count] times. Default: infinite.\n\
  -p, --pid=pid Only display the given process information.\n\
  -h, --help Display this information and exit.\n\
"
enum option_index
{
    t_opt = 0,
    c_opt,
    p_opt,
    h_opt,
};

int main(int argc, char *argv[])
{
    int8_t option[4]={0};
    struct option longopts[] = {
        {"time-interval", required_argument, NULL, 't'},
        {"count", required_argument, NULL, 'c'},
        {"pid", required_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};
    int32_t opt = 0;
    int64_t time_interval = 5;
    int64_t count = -1;
    int64_t pid = -1;
    
    while ((opt = getopt_long(argc, argv, "t:c:p:h", longopts, NULL)) != -1)
    {
        char *err_num = NULL;
        switch (opt)
        {
            case 't':
                option[t_opt]++;
                time_interval = strtol(optarg,&err_num,10);
                if(*err_num!='\0')
                {
                    fprintf(stdout,"Invalid time interval.\n");
                    return -1;
                }
                if(time_interval<=0)
                {
                    fprintf(stdout,"Invalid time interval.\n");
                    return -1;
                }
                if(option[t_opt]>1)
                {
                    fprintf(stdout,"Duplicate option.\n");
                    return -1;
                }
                break;
            case 'c':
                option[c_opt]++;
                count = strtol(optarg,&err_num,10);
                if(*err_num!='\0')
                {
                    fprintf(stdout,"Invalid count.\n");
                    return -1;
                }
                if(count<=0)
                {
                    fprintf(stdout,"Invalid count.\n");
                    return -1;
                }
                if(option[c_opt]>1)
                {
                    fprintf(stdout,"Duplicate option.\n");
                    return -1;
                }
                break;
            case 'p':
                option[p_opt]++;
                pid = strtol(optarg,&err_num,10);
                if(*err_num!='\0')
                {
                    fprintf(stdout,"Invalid pid.\n");
                    return -1;
                }
                if(pid<=0)
                {
                    fprintf(stdout,"Invalid pid.\n");
                    return -1;
                }
                if(option[p_opt]>1)
                {
                    fprintf(stdout,"Duplicate option.\n");
                    return -1;
                }
                break;
            case 'h':
                option[h_opt]++;
                printf("%s",HELP_MSG);
                return 0;
            default:
                fprintf(stdout,"Invalid option.\n");
                fprintf(stdout,"%s",HELP_MSG);
                return -1;
        }
    }
    for(int32_t i = 0; count==-1?1:i<count;i++)
    {
        if(pid!=-1)
        {
            char path[100];
            snprintf(path,100,"/proc/%ld",pid);
            if(access(path,F_OK)==-1)
            {
                fprintf(stdout,"No such process.\n");
                return -1;
            }
            system("clear");
            char command[1000] = {0};
            snprintf(command,1000,"top -o -PID -b -n 1 -p%ld | awk '{ printf \"%%-6s %%-15s %%5s %%5s%%%%   %%-10s\\n\",$1,$12,$8,$9,$5*1024}' | tail -n +8",pid);
            printf("PID    NAME              state CPU  MEM\n");
            system(command);
        }
        else
        {
            system("clear");
            printf("PID    NAME              state CPU    MEM\n");
            system("top -o -PID -b -n 1 | awk '{ printf \"%-6s %-15s %5s %5s%%   %-10s\\n\",$1,$12,$8,$9,$5*1024}' | head -n -4 | tail -n +8");
        }
        sleep(time_interval);
    }
    //top -o -PID -b -n 1 | awk '{ printf "%-6s %-15s %5s %5s%% %-10s\n",$1,$12,$8,$9,$7*1024}' | head -n -4 | tail -n +8
    // system("top -o -PID -b -n 1 | awk '{ printf \"%-6s %-15s %5s %5s%% %-10s\\n\",$1,$12,$8,$9,$7*1024}' | head -n -4 | tail -n +8");
    return 0;
}