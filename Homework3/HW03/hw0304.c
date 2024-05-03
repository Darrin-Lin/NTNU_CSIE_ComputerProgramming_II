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

// #define _DEBUG_ 1

#define HELP_MSG "\
  -p, --pid=dosbox-staging_pid       Give program the dosbox-staging's pid.\n\
  -a, --adress=game_memory_adress    Give program the game's memory adress(Hex, start with 0x).\n\
  -h, --help                         Show help message.\n\
Both -p and -m is necessory.\n\
"

// #if defined(_DEBUG_)
// #define pterr(fmt, ...)                 \
//     fprintf(stderr, "%s:%d:%s(): " fmt, \
//             __FILE__, __LINE__, __func__, __VA_ARGS__)
// #else
// #define pterr(fmt, ...) \
//     {                   \
//     }
// #endif

enum option_index
{
    p_opt = 0,
    a_opt,
    h_opt,
};

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
    char pid[1024] = {0};
    // pterr("err123");
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
                fprintf(stdout, "Error: %s is not a decimal number.\n");
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
                fprintf(stdout, "Error: %s is not a hex number.\n");
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
    uint64_t file_size = 0;
    struct stat st;
    if (fstat(game, &st) < 0)
    {
        fprintf(stdout, "Error: state error\n");
        close(game);
        return -1;
    }
    file_size = st.st_size;
    if (game == -1)
    {
        fprintf(stdout, "Can't open %s", path);
        goto err_arg;
    }
    void *adress = (void *)adress_num;
    uint8_t *memory = (int8_t *)mmap(adress, file_size, 255| PROT_READ | PROT_WRITE, MAP_SHARED, game, 0);
    // printf("-%d\n", memory[0]);
    char test[10];
    read(game,&test,5);
    printf("%d\n",test[0]);
    return 0;

err_arg:
    return -1;
}