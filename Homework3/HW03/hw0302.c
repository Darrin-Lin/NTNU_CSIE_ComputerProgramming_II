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
#include "myvector.h"

#define DEBUG 1
#define HELP_MSG "\
Usage: hw0302 [options] ... [files] ...\n\
  -f, --function=func Trace the func usage in [Files].\n\
  -i, --include=File Trace all functions listed in the header file in [Files].\n\
  -l, --linum Display the line number.\n\
  -c, --code Display the code.\n\
  -h, --help Display this information and exit.\n\n\
-f and -i are exclusive and must be at least one.\n\
"

int32_t get_function_names(FILE *header_file, char **func);
// return vector_size


int main(int argc, char *argv[])
{
    enum option_index
    {
        f_opt = 0,
        i_opt,
        l_opt,
        c_opt,
        h_opt,
    };
    int8_t option[5] = {0};
    struct option longopts[] = {
        {"function", required_argument, NULL, 'f'},
        {"include", required_argument, NULL, 'i'},
        {"linum", no_argument, NULL, 'l'},
        {"code", no_argument, NULL, 'c'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};
    int32_t opt = 0;
    char func[1024];
    char include_file[1024];
    while ((opt = getopt_long(argc, argv, "f:i:lch", longopts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'f':
            option[f_opt]++;
            if (option[i_opt])
            {
                fprintf(stdout, "Error: -f and -i are exclusive and must be at least one.\n");
                goto err_arg;
            }
            if (option[f_opt] > 1)
            {
                fprintf(stdout, "Error: -f can only be used once.\n");
                goto err_arg;
            }
            strcpy(func, optarg);
            break;
        case 'i':
            option[i_opt]++;
            if (option[f_opt])
            {
                fprintf(stdout, "Error: -f and -i are exclusive and must be at least one.\n");
                goto err_arg;
            }
            if (option[i_opt] > 1)
            {
                fprintf(stdout, "Error: -i can only be used once.\n");
                goto err_arg;
            }
            strcpy(include_file, optarg);
            break;
        case 'l':
            option[l_opt]++;
            if (option[l_opt] > 1)
            {
                fprintf(stdout, "Error: -l can only be used once.\n");
                goto err_arg;
            }
            break;
        case 'c':
            option[c_opt]++;
            if (option[c_opt] > 1)
            {
                fprintf(stdout, "Error: -c can only be used once.\n");
                goto err_arg;
            }
            break;
        case 'h':
            if (option[f_opt] || option[i_opt] || option[l_opt] || option[c_opt])
            {
                fprintf(stdout, "Error: -h cannot be used with other options.\n");
                goto err_arg;
            }
            option[h_opt]++;
            fprintf(stdout, HELP_MSG);
            return 0;
        default:
            fprintf(stdout, "Error: Invalid argument.\n");
            goto err_arg;
            break;
        }
    }
    if (DEBUG)
    {
        //print index
        fprintf(stderr,"argv%d\n",optind);
        fprintf(stderr,"argc%s\n",argv[optind]);
    }
    if (!option[f_opt] && !option[i_opt])
    {
        fprintf(stdout, "Error: -f and -i must be at least one.\n");
        goto err_arg;
    }
    FILE *file = fopen(argv[optind],"r");
    if (file == NULL)
    {
        fprintf(stdout, "Error: cannot open file.\n");
        goto err_arg;
    }
    char line[1024];
    int32_t line_num = 0;

    return 0;
err_arg:

    return -1;
}

int32_t get_function_names(FILE *header_file, char **func)
{
    char line[1024];
    int32_t line_num = 0;
    
    
}