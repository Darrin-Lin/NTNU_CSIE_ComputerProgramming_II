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

#define DEBUG 0
#define HELP_MSG "\
Usage: hw0302 [options] ... [files] ...\n\
  -f, --function=func   Trace the func usage in [Files].\n\
  -i, --include=File    Trace all functions listed in the header file in [Files].\n\
  -l, --linum       Display the line number.\n\
  -c, --code        Display the code.\n\
  -h, --help        Display this information and exit.\n\n\
-f and -i are exclusive and must be at least one.\n\
"

int32_t get_function_names(FILE *header_file, char *func);
int32_t print_function_count(FILE *file, char *func, int8_t option[5]);
// return vector_size
enum option_index
{
    f_opt = 0,
    i_opt,
    l_opt,
    c_opt,
    h_opt,
};
int main(int argc, char *argv[])
{

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
            // if (option[f_opt] || option[i_opt] || option[l_opt] || option[c_opt])
            // {
            //     fprintf(stdout, "Error: -h cannot be used with other options.\n");
            //     goto err_arg;
            // }
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
        // print index
        fprintf(stderr, "argv%d\n", optind);
        fprintf(stderr, "argc%s\n", argv[optind]);
    }

    int32_t file_n = 0;
    file_n = argc - optind;
    if (!option[f_opt] && !option[i_opt])
    {
        fprintf(stdout, "Error: -f and -i must be at least one.\n");
        goto err_arg;
    }
    char *file_name = NULL;
    file_name = argv[optind];
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        fprintf(stdout, "Error: cannot open file.\n");
        goto err_arg;
    }

    if (option[f_opt])
    {
        fprintf(stdout, "%s\n", func);
        for (int32_t i = 0; i < file_n; i++)
        {
            file_name = argv[optind + i];
            file = fopen(file_name, "r");
            if (file == NULL)
            {
                fprintf(stdout, "Error: file %s cannot open.\n", file_name);
                goto err_arg;
            }
            fprintf(stdout, "  %s ", file_name);
            print_function_count(file, func, option);
            fclose(file);
        }
    }
    else if (option[i_opt])
    {
        FILE *header_file = fopen(include_file, "r");
        if (header_file == NULL)
        {
            fprintf(stdout, "Error: cannot open %s.\n", include_file);
            goto err_arg;
        }
        if (get_function_names(header_file, func) == -1)
        {
            fprintf(stdout, "Error: cannot find any function.\n");
            goto err_arg;
        }
        do
        {
            if (DEBUG)
                fprintf(stderr, "func_out: %s\n", func);
            fprintf(stdout, "%s\n", func);
            for (int32_t i = 0; i < file_n; i++)
            {
                file_name = argv[optind + i];
                file = fopen(file_name, "r");
                if (file == NULL)
                {
                    fprintf(stdout, "Error: file %s cannot open.\n", file_name);
                    goto err_arg;
                }
                fprintf(stdout, "  %s ", file_name);
                print_function_count(file, func, option);
                fclose(file);
            }
        } while (get_function_names(header_file, func) != -1);
    }

    return 0;
err_arg:

    return -1;
}

int32_t get_function_names(FILE *header_file, char *func_find)
{
    char line[1024];
    while (!feof(header_file))
    {
        fgets(line, 1024, header_file);
        while (strlen(line) > 1 && line[strlen(line) - 2] == '\\')
        {
            char tmp_chr[1024];
            fgets(tmp_chr, 1024, header_file);
            line[strlen(line) - 2] = '\0';
            strcat(line, tmp_chr);
        }
        // if (strstr(line, "int") != NULL || strstr(line, "void") != NULL || strstr(line, "char") != NULL || strstr(line, "float") != NULL || strstr(line, "double") != NULL)
        // {
        if (strstr(line, "__") == NULL)
            if (strstr(line, "(") != NULL)
            {
                if (strstr(line, ")") != NULL)
                {
                    if (strstr(line, ";") != NULL)
                    {
                        char *p = strtok(line, "(");
                        char *q = strtok(line, " ");
                        q = strtok(NULL, " ");
                        if (strrchr(q, '*') != NULL)
                        {
                            q = strrchr(q, '*') + 1;
                        }

                        strcpy(func_find, q);
                        return 0;
                    }
                }
                // }
            }
    }
    return -1;
}

int32_t print_function_count(FILE *file, char *func, int8_t option[5])
{
    char line[1024];
    int32_t line_num = 0;
    int32_t count = 0;
    FILE *tmp_out = fopen("tmp.txt", "w");
    if (tmp_out == NULL)
    {
        fprintf(stdout, "Error: cannot open file.\n");
        return -1;
    }
    int8_t annotate = 0;
    while (!feof(file))
    {
        fgets(line, 1024, file);
        int32_t tmp_line_num = line_num;
        char print_line[1024];
        strcpy(print_line, line);
        while (strlen(line) > 1 && line[strlen(line) - 2] == '\\')
        {
            char tmp_chr[1024];
            fgets(tmp_chr, 1024, file);
            line[strlen(line) - 2] = '\0';
            strcat(line, tmp_chr);
            strcat(print_line, tmp_chr);
            tmp_line_num++;
        }

        line_num++;
        if (annotate)
        {
            if (strstr(line, "*/") == NULL)
                continue;
        }
        
        if (strstr(line, "/*") != NULL)
        {
            if (strstr(line, "*/") != NULL)
            {
                char *end = NULL;
                int32_t comment_l = (strstr(line, "*/") - strstr(line, "/*"));

                if (comment_l > 0)
                {
                    while (comment_l > 0)
                    {
                        for (char *i = strstr(line, "/*"); *(i + comment_l + 2) != '\0'; i = i)
                        {
                            *i = *(i + comment_l + 2);
                            end = i;
                            i++;
                        }
                        *(end + 1) = '\0';
                        if (DEBUG)
                        {
                            fprintf(stderr, "%s", line);
                        }
                        if (strstr(line, "*/") == NULL || strstr(line, "/*") == NULL)
                            break;
                        comment_l = (strstr(line, "*/") - strstr(line, "/*"));
                    }
                }
                else
                {
                    *(strstr(line, "/*")) = '\0';
                    char tmp_chr[1024];
                    strcpy(tmp_chr, line);
                    strcpy(line, strstr(tmp_chr, "*/") + 2);
                }
                annotate = 0;
            }
            else
            {
                *(strstr(line, "/*")) = '\0';
                annotate = 1;
            }
        }
        if (strstr(line, "*/") != NULL)
        {
            annotate = 0;
            char tmp_chr[1024];
            strcpy(tmp_chr, line);
            strcpy(line, strstr(tmp_chr, "*/") + 2);
        }
        if (strstr(line, "//") != NULL)
        {
            *(strstr(line, "//")) = '\0';
        }
        int32_t count_quotation = 0;
        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] == '\"')
            {
                count_quotation++;
            }
        }
        if (count_quotation)
        {
            if (DEBUG)
            {
                fprintf(stderr, "count_quotation: %d\n", count_quotation);
            }
            while (count_quotation % 2 == 0 && count_quotation)
            {
                int32_t quotation_l = 0;
                quotation_l = strchr(strchr(line, '"') + 1, '"') - strchr(line, '"');

                char *end_ptr = NULL;
                for (char *i = strchr(line, '"'); *(i + quotation_l + 1) != '\0'; i = i)
                {
                    *i = *(i + quotation_l + 1);
                    i++;
                    end_ptr = i;
                }
                count_quotation -= 2;

                *(end_ptr) = '\0';
            }
        }
        if (strstr(line, func) != NULL)
        {
            count++;
            if (option[l_opt])
            {
                fprintf(tmp_out, "    line %d", line_num);
                if (option[c_opt])
                {
                    fprintf(tmp_out, ": ");
                }
                else
                {
                    fprintf(tmp_out, "\n");
                }
            }
            if (option[c_opt])
            {
                if (!option[l_opt])
                    fprintf(tmp_out, "    ");
                if (line[0] == ' ')
                {
                    char *ptr_line = print_line;
                    while (*(ptr_line) == ' ')
                    {
                        ptr_line++;
                    }
                    fprintf(tmp_out, "%s", ptr_line);
                }
                else
                    fprintf(tmp_out, "%s", print_line);
            }
        }
        line_num = tmp_line_num;
    }
    fclose(tmp_out);
    fprintf(stdout, "(count: %d)\n", count);
    char new_line[1024];
    tmp_out = fopen("tmp.txt", "r");
    while (!feof(tmp_out))
    {
        fgets(new_line, 1024, tmp_out);
        if (new_line[0] != '\n' && !feof(tmp_out))
        {
            fprintf(stdout, "%s", new_line);
        }
    }
    fclose(tmp_out);
    if (!DEBUG)
        remove("tmp.txt");
    return 0;
}