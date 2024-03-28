#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// #include "hahaha.h"

static int8_t get_words(char *input, char output[5])
{
    if (input == NULL)
        return -1;
    for (int32_t i = 0; i < strlen(input); i++)
    {
        if (strchr("\'-", input[i]) != NULL)
        {
            return -1;
        }
    }
    if (strchr(input, '/') == NULL)
    {
        if (strlen(input) == 6 && input[5] == '\n')
        {
            input[5] = '\0';
            strcpy(output, input);
            return 0;
        }
        else
        {
            return -1;
        }
    }
    char *token = strtok(input, "/");
    if (token == NULL)
        return -1;
    if (strlen(token) != 5)
    {
        return -1;
    }
    strcpy(output, token);
    return 0;
}

int main()
{
    FILE *dic = fopen("en_US.dic", "r");
    FILE *five_chr_dic = fopen("five_chr_dic.txt", "w");
    if (dic == NULL)
    {
        return -1;
    }
    char line[100];
    while (!feof(dic))
    {
        fgets(line, 100, dic);
        char output[5];
        if (get_words(line, output) == 0)
        {
            fprintf(five_chr_dic, "%s\n", output);
        }
    }
    fclose(dic);
    fclose(five_chr_dic);
    
    
    // char test[100];
    // strcpy(test, pp);
    // strncat(test, "output_words", 100);
    // strncat(test, ppp, 100);
    // haha(test);

    return 0;
}
/*
A: 8.2
B: 1.5
C: 2.8
D: 4.3
E: 12.7
F: 2.2
G: 2.0
H: 6.1
I: 7.0
J: 0.15
K: 0.77
L: 4.0
M: 2.4
N: 6.7
O: 7.5
P: 1.9
Q: 0.095
R: 6.0
S: 6.3
T: 9.1
U: 2.8
V: 0.98
W: 2.4
X: 0.15
Y: 2.0
Z: 0.074
*/