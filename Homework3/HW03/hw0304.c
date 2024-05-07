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

#define _DEBUG_

#define HELP_MSG "\
  -p, --pid=dosbox-staging_pid       Give program the dosbox-staging's pid.\n\
  -a, --adress=game_memory_adress    Give program the game's memory adress(Hex, start with 0x).\n\
  -h, --help                         Show help message.\n\
Both -p and -m is necessory.\n\
"

#if defined(_DEBUG_)
#define debug(fmt, ...) \
    fprintf(stderr, "%s:%d:%s():\n" fmt, __FILE__, __LINE__, __func__)
#else
#define debug(fmt, ...) \
    {                   \
    }
#endif

enum option_index
{
    p_opt = 0,
    a_opt,
    h_opt,
};

int8_t print_80_byte(int32_t file)
{
    char buffer[100] = {0};
    read(file, buffer, 80);
    for (int i = 0; i < 80; i++)
    {
        if (i % 16 == 0)
            fprintf(stderr, "\n");
        if (i % 16 == 8)
            fprintf(stderr, "| ");
        fprintf(stderr, "0x%hhx ", buffer[i]);
    }
    fprintf(stderr, "\n-----\n");
    return 0;
}

int main(int argc, char *argv[])
{
    struct option longopts[] = {
        {"pid", required_argument, NULL, 'p'},
        {"adress", required_argument, NULL, 'a'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};
    int8_t option[3] = {0};
    int32_t opt = 0;
    uint64_t adress_num = 0;
    char pid[512] = {0};
    while ((opt = getopt_long(argc, argv, "p:a:h", longopts, NULL)) != -1)
    {
        char *num_err = NULL;
        switch (opt)
        {
        case 'p':
            num_err = NULL;
            option[p_opt]++;
            if (option[p_opt] > 1)
            {
                fprintf(stdout, "Error: -p can only be used once.\n");
                goto err_arg;
            }
            int64_t num = strtol(optarg, &num_err, 10);
            if (*num_err != '\0')
            {
                fprintf(stdout, "Error: %s is not a decimal number.\n", optarg);
                goto err_arg;
            }
            if (num < 0)
            {
                fprintf(stdout, "Error: pid can't less than zero.");
            }
            strcpy(pid, optarg);
            break;

        case 'a':
            num_err = NULL;
            option[a_opt]++;
            if (option[a_opt] > 1)
            {
                fprintf(stdout, "Error: -p can only be used once.\n");
                goto err_arg;
            }
            if (strstr(optarg, "0x") != optarg)
            {
                fprintf(stdout, "Error: Please enter the hex start with 0x.\n");
                goto err_arg;
            }
            adress_num = strtol(optarg, &num_err, 16);
            if (*num_err != '\0')
            {
                fprintf(stdout, "Error: %s is not a hex number.\n", optarg);
                goto err_arg;
            }
            break;

        case 'h':
            fprintf(stdout, HELP_MSG);
            return 0;
            break;
        default:
            fprintf(stdout, "Error: Invalid argument.\n");
            goto err_arg;
            break;
        }
    }
    if (!(option[p_opt] && option[a_opt]))
    {
        fprintf(stdout, "Error: -p and -a is necessary.\n");
        goto err_arg;
    }
    int32_t game = 0;
    char path[1024] = {0};
    sprintf(path, "/proc/%s/mem", pid);
    game = open(path, O_RDWR);
    if (game == -1)
    {
        fprintf(stdout, "Can't open %s", path);
        goto err_arg;
    }
    int64_t file_size = 0;
    struct stat st;
    if (fstat(game, &st) < 0)
    {
        fprintf(stdout, "Error: state error\n");
        close(game);
        return -1;
    }
    perror("");
    file_size = st.st_size;
    if (game == -1)
    {
        fprintf(stdout, "Can't open %s", path);
        goto err_arg;
    }
    lseek(game, adress_num, SEEK_SET);
    printf("size: %ld\n",_SC_PAGE_SIZE);
    char buffer[96] = {0};
    // 16mb = 1<<24
    printf("%lx\n", adress_num);
    printf("%d\n", game);
    // int8_t *adress_map = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, game, adress_num*_SC_PAGESIZE);//No such device

    // if (adress_map == MAP_FAILED)
    // {
    //     printf("map_error\n");
    //     perror("");
    //     return -1;
    // }
    // printf("%p\n", adress_map);
    // printf("%hhu\n", adress_map[2]);
    uint16_t now_hp, now_mp, max_hp, max_mp;

    printf("hp:");
    scanf("%hu%hu", &now_hp, &max_hp);
    printf("mp:");
    scanf("%hu%hu", &now_mp, &max_mp);

    int64_t idx = 0;
    while (read(game, buffer, 16) > 0)
    {
        // if(idx==0)
        // {
        //     printf("T?F%d",*((uint16_t *)(buffer + 2)) ==0xf000);
        //     fprintf(stderr, "%lx:\n", idx);
        //     for (int32_t i = 0; i < 16; i++)
        //     {
        //         if (i == 8)
        //             fprintf(stderr, "| ");
        //         fprintf(stderr, "0x%hhx ", buffer[i]);
        //     }
        //     fprintf(stderr, "\n");
        // }
        if (*((uint16_t *)(buffer + 8)) == now_hp && *((uint16_t *)(buffer + 10)) == max_hp && *((uint16_t *)(buffer + 12)) == now_mp && *((uint16_t *)(buffer + 14)) == max_mp)
        {
            fprintf(stderr, "%lx:\n", idx);
            for (int32_t i = 0; i < 16; i++)
            {
                if (i == 8)
                    fprintf(stderr, "| ");
                fprintf(stderr, "0x%hhx ", buffer[i]);
            }
            fprintf(stderr, "\n");
        }
        idx++;
    }
    // for(int32_t i =0;i<1<<20;i++)
    // {
    //     fprintf(stderr,"%d\n",i);
    // print_80_byte(game);
    // }
    close(game);
    return 0;

err_arg:
    return -1;
err_close:
    close(game);
    return -1;
}