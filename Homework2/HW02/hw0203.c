#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// #include "hahaha.h"

#define fptf fprintf
#define DEBUG 0

const double freq[26] = {8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.15, 0.77, 4.0, 2.4, 6.7, 7.5, 1.9, 0.095, 6.0, 6.3, 9.1, 2.8, 0.98, 2.4, 0.15, 2.0, 0.074};

static int8_t get_5_chr_words(char *input, char output[6]);
static int32_t rm_words(char input, int32_t pos, int8_t t);
static double find_highest(char output[6]);

int main()
{
    FILE *dic = fopen("en_US.dic", "r");
    FILE *five_chr_dic = fopen("five_chr_dic.txt", "w");
    if (dic == NULL)
    {
        fclose(five_chr_dic);
        return -1;
    }
    char line[100];
    while (!feof(dic))
    {
        fgets(line, 100, dic);
        char output[6];
        if (get_5_chr_words(line, output) == 0)
        {
            fprintf(five_chr_dic, "%s\n", output);
        }
    }
    fclose(dic);
    fclose(five_chr_dic);

    /*char test[100];
    strcpy(test, pp);
    strncat(test, "output_words", 100);
    strncat(test, ppp, 100);
    haha(test);*/
    char guess[6];
    sscanf("EERIE", "%s", guess);
    char usr_input[1024] = {0};
    char answer[6] = {' ', ' ', ' ', ' ', ' ', '\0'};
    int8_t last_appear[26] = {0};

    while (1)
    {
        printf("Advice:   ");
        printf("%s\n", guess);
        printf("Feedback: ");
        // scanf("%s", usr_input);
        fgets(usr_input, 1024, stdin);
        if (strlen(usr_input) != 6)
        {
            printf("Wrong Input\n");
            goto err_cleanup;
        }
        usr_input[5] = '\0';
        for (int32_t i = 0; i < 5; i++)
        {
            if (strchr("BYG", usr_input[i]) == NULL)
            {
                printf("Wrong Input\n");
                goto err_cleanup;
            }
        }
        int32_t words_num = 0;
        int8_t now_appear[26] = {0};
        for (int32_t i = 0; i < 5; i++)
        {
            if (usr_input[i] == 'G')
            {
                words_num = rm_words(guess[i], i, 0);
                if (now_appear[guess[i] - 'A'] == -1)
                    now_appear[guess[i] - 'A'] = 0;
                now_appear[guess[i] - 'A']++;
                answer[i] = guess[i];
            }
            else if (usr_input[i] == 'Y')
            {
                words_num = rm_words(guess[i], -1, 0);
                if (now_appear[guess[i] - 'A'] == -1)
                    now_appear[guess[i] - 'A'] = 0;
                now_appear[guess[i] - 'A']++;
            }
            else if (usr_input[i] == 'B' && now_appear[guess[i] - 'A'] == 0)
            {
                now_appear[guess[i] - 'A'] = -1;
            }
        }
        for (int32_t i = 0; i < 26; i++)
        {
            if ((now_appear[i] > last_appear[i] && last_appear[i] != -1) || now_appear[i] == -1)
            {
                if (now_appear[i] == -1 && last_appear[i] > 0)
                {
                    printf("Wrong Input\n");
                    goto err_cleanup;
                }
                if (last_appear[i] == -1 && now_appear[i] > 0)
                {
                    printf("Wrong Input\n");
                    goto err_cleanup;
                }
                last_appear[i] = now_appear[i];
            }
            if (DEBUG)
                fptf(stderr, "%d ", last_appear[i]);
        }
        for (int32_t i = 0; i < 26; i++)
        {
            if (last_appear[i])
            {
                words_num = rm_words(i + 'A', -1, last_appear[i]);
            }
        }
        if (words_num == 0)
        {
            printf( “No Advice\n”);
            goto err_cleanup;
        }
        else if (words_num == 1 || strchr(answer, ' ') == NULL)
        {
            printf("Congratulations!!\n");
            if (!DEBUG)
                remove("five_chr_dic.txt");
            return 0;
        }
        else
        {
            find_highest(guess);
        }
    }
    if (!DEBUG)
        remove("five_chr_dic.txt");
    return 0;
err_cleanup:
    if (!DEBUG)
        remove("five_chr_dic.txt");
    return -1;
}

static int8_t get_5_chr_words(char *input, char output[6])
{
    if (input == NULL)
        return -1;
    for (uint32_t i = 0; i < strlen(input); i++)
    {
        if (input[i] == '\n' || input[i] == '/')
            break;
        if (!((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z')))
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
            for (int32_t i = 0; i < 5; i++)
            {
                if (output[i] >= 'a' && output[i] <= 'z')
                {
                    output[i] = output[i] - 'a' + 'A';
                }
            }
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
    for (int32_t i = 0; i < 5; i++)
    {
        if (output[i] >= 'a' && output[i] <= 'z')
        {
            output[i] = output[i] - 'a' + 'A';
        }
    }
    return 0;
}
static int32_t rm_words(char input, int32_t pos, int8_t t) // t = 0 don't know/care, t<0 not in, t>0 in
{
    FILE *dic = fopen("five_chr_dic.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (dic == NULL)
    {
        return -1;
    }

    int32_t count = 0;
    if (DEBUG)
    {
        fptf(stderr, "input: %c, pos: %d, t: %d\n", input, pos, t);
    }
    while (!feof(dic))
    {
        char line[7] = {0};
        fgets(line, 7, dic);
        if (pos == -1)
        {

            if (t == 0)
            {
                if (strchr(line, input) != NULL)
                {
                    fprintf(temp, "%s", line);
                    count++;
                }
            }
            else if (t == -1)
            {
                if (strchr(line, input) == NULL)
                {
                    fprintf(temp, "%s", line);
                    count++;
                }
            }
            else
            {
                int32_t ct = 0;
                for (int32_t i = 0; i < 5; i++)
                {

                    if (line[i] == input)
                    {
                        ct++;
                    }
                }
                if (ct == t)
                {
                    fprintf(temp, "%s", line);
                    count++;
                }
            }
        }
        else
        {
            if (line[pos] == input)
            {
                fprintf(temp, "%s", line);
                if (DEBUG)
                    fptf(stderr, "%s", line);
                count++;
            }
        }
    }

    fclose(dic);
    fclose(temp);
    remove("five_chr_dic.txt");
    rename("temp.txt", "five_chr_dic.txt");
    return count;
}

static double find_highest(char output[6])
{
    FILE *dic = fopen("five_chr_dic.txt", "r");
    if (dic == NULL)
    {
        return -1;
    }
    char line[7];
    double highest = 0;
    char highest_word[6];
    while (!feof(dic))
    {
        fgets(line, 7, dic);
        line[5] = '\0';
        double sum = 0;
        for (int32_t i = 0; i < 5; i++)
        {
            sum += freq[line[i] - 'A'];
        }
        if (sum - highest > 1e-6)
        {
            highest = sum;
            strcpy(highest_word, line);
        }
    }
    strcpy(output, highest_word);
    return highest;
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