#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "myvector.h"

static int32_t get_value(char *input, char **home_team, char **away_team, uint16_t *FTHG, uint16_t *FTAG)
{
    if (input == NULL)
        return -1;
    char *token = strtok(input, ",");
    if (token == NULL)
        return -1;
    char *tmp_save = NULL;
    tmp_save = token;
    token = strtok(NULL, ",");
    if (token == NULL)
        return -1;
    *home_team = token;
    token = strtok(NULL, ",");
    if (token == NULL)
        return -1;
    *away_team = token;
    token = strtok(NULL, ",");
    if (token == NULL)
        return -1;
    *FTHG = (uint16_t)strtol(token, &tmp_save, 10);
    token = strtok(NULL, ",");
    if (token == NULL)
        return -1;
    *FTAG = (uint16_t)strtol(token, &tmp_save, 10);
    return 0;
}

int main()
{
    char file_name[100];
    printf("Please enter the data file name: ");
    fgets(file_name, 100, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *file = fopen(file_name, "r");
    if(file==NULL)
    {
        printf("No file.\n");
        return -1;
    }
    char **teams = (char **)Vector_create_ptr(0);
    int64_t *win = Vector_create(0);
    int64_t *draw = Vector_create(0);
    int64_t *lose = Vector_create(0);
    int64_t *GF = Vector_create(0);
    int64_t *GA = Vector_create(0);
    size_t team_num = 0;
    if (file == NULL)
    {
        return -1;
    }
    char line[200];
    fgets(line, 200, file);
    while (!feof(file))
    {
        char *home_team = NULL;
        char *away_team = NULL;
        char *get_return = NULL;
        get_return = fgets(line, 200, file);
        if (get_return == NULL)
        {
            continue;
        }
        uint16_t FTHG = 0;
        uint16_t FTAG = 0;
        if (get_value(line, &home_team, &away_team, &FTHG, &FTAG) == -1)
        {
            fprintf(stderr, "Error\n");
            return -1;
        }
        int16_t ht_flag = 0;
        int16_t at_flag = 0;
        for (size_t i = 0; i < team_num; i++)
        {
            if (strcmp(teams[i], home_team) == 0)
            {
                GF[i] += FTHG;
                GA[i] += FTAG;
                if (FTHG > FTAG)
                {
                    win[i]++;
                }
                else if (FTHG == FTAG)
                {
                    draw[i]++;
                }
                else
                {
                    lose[i]++;
                }
                ht_flag = 1;
            }
            else if (strcmp(teams[i], away_team) == 0)
            {
                GF[i] += FTAG;
                GA[i] += FTHG;
                if (FTHG < FTAG)
                {
                    win[i]++;
                }
                else if (FTHG == FTAG)
                {
                    draw[i]++;
                }
                else
                {
                    lose[i]++;
                }
                at_flag = 1;
            }
        }
        if (ht_flag == 0)
        {
            char *char_tmp = (char *)calloc(strlen(home_team) + 1, sizeof(char));
            if (char_tmp == NULL)
            {
                return -1;
            }
            strcpy(char_tmp, home_team);
            Vector_push_back_ptr((void ***)&teams, char_tmp, team_num);
            Vector_push_back(&win, FTHG > FTAG ? 1 : 0, team_num);
            Vector_push_back(&draw, FTHG == FTAG ? 1 : 0, team_num);
            Vector_push_back(&lose, FTHG < FTAG ? 1 : 0, team_num);
            Vector_push_back(&GF, FTHG, team_num);
            Vector_push_back(&GA, FTAG, team_num);
            team_num++;
        }
        if (at_flag == 0)
        {
            char *char_tmp = (char *)calloc(strlen(away_team) + 1, sizeof(char));
            if (char_tmp == NULL)
            {
                return -1;
            }
            strcpy(char_tmp, away_team);
            Vector_push_back_ptr((void ***)&teams, char_tmp, team_num);
            Vector_push_back(&win, FTHG < FTAG ? 1 : 0, team_num);
            Vector_push_back(&draw, FTHG == FTAG ? 1 : 0, team_num);
            Vector_push_back(&lose, FTHG > FTAG ? 1 : 0, team_num);
            Vector_push_back(&GF, FTAG, team_num);
            Vector_push_back(&GA, FTHG, team_num);
            team_num++;
        }
    }
    fclose(file);
    int16_t GD[team_num];
    for (size_t i = 0; i < team_num; i++)
    {
        GD[i] = GF[i] - GA[i];
    }
    int16_t pts[team_num];
    for (size_t i = 0; i < team_num; i++)
    {
        pts[i] = win[i] * 3 + draw[i];
    }
    // sort
    for (size_t i = 0; i < team_num; i++)
    {
        for (size_t j = i + 1; j < team_num; j++)
        {
            if (pts[i] < pts[j])
            {
                int16_t tmp = pts[i];
                pts[i] = pts[j];
                pts[j] = tmp;
                tmp = win[i];
                win[i] = win[j];
                win[j] = tmp;
                tmp = draw[i];
                draw[i] = draw[j];
                draw[j] = tmp;
                tmp = lose[i];
                lose[i] = lose[j];
                lose[j] = tmp;
                tmp = GF[i];
                GF[i] = GF[j];
                GF[j] = tmp;
                tmp = GA[i];
                GA[i] = GA[j];
                GA[j] = tmp;
                tmp = GD[i];
                GD[i] = GD[j];
                GD[j] = tmp;
                char *tmp_char = teams[i];
                teams[i] = teams[j];
                teams[j] = tmp_char;
            }
            else if (pts[i] == pts[j])
            {
                if (GD[i] < GD[j])
                {
                    int16_t tmp = pts[i];
                    pts[i] = pts[j];
                    pts[j] = tmp;
                    tmp = win[i];
                    win[i] = win[j];
                    win[j] = tmp;
                    tmp = draw[i];
                    draw[i] = draw[j];
                    draw[j] = tmp;
                    tmp = lose[i];
                    lose[i] = lose[j];
                    lose[j] = tmp;
                    tmp = GF[i];
                    GF[i] = GF[j];
                    GF[j] = tmp;
                    tmp = GA[i];
                    GA[i] = GA[j];
                    GA[j] = tmp;
                    tmp = GD[i];
                    GD[i] = GD[j];
                    GD[j] = tmp;
                    char *tmp_char = teams[i];
                    teams[i] = teams[j];
                    teams[j] = tmp_char;
                }
                else if (GD[i] == GD[j])
                {
                    if (GF[i] < GF[j])
                    {
                        int16_t tmp = pts[i];
                        pts[i] = pts[j];
                        pts[j] = tmp;
                        tmp = win[i];
                        win[i] = win[j];
                        win[j] = tmp;
                        tmp = draw[i];
                        draw[i] = draw[j];
                        draw[j] = tmp;
                        tmp = lose[i];
                        lose[i] = lose[j];
                        lose[j] = tmp;
                        tmp = GF[i];
                        GF[i] = GF[j];
                        GF[j] = tmp;
                        tmp = GA[i];
                        GA[i] = GA[j];
                        GA[j] = tmp;
                        tmp = GD[i];
                        GD[i] = GD[j];
                        GD[j] = tmp;
                        char *tmp_char = teams[i];
                        teams[i] = teams[j];
                        teams[j] = tmp_char;
                    }
                }
            }
        }
    }
    // print
    printf("    Team          W\tD\tL\tGF\tGA\tGD\tPts\n");
    for (size_t i = 0; i < team_num; i++)
    {
        if (i < 9)
            printf("0%ld) ", i + 1);
        else
            printf("%2ld) ", i + 1);
        printf("%s", teams[i]);
        for (size_t j = 0; j < 14 - strlen(teams[i]); j++)
            printf(" ");
        if (strlen(teams[i]) < 8)
            printf("%ld\t%ld\t%ld\t%ld\t%ld\t", win[i], draw[i], lose[i], GF[i], GA[i]);
        else
            printf("%ld\t%ld\t%ld\t%ld\t%ld\t", win[i], draw[i], lose[i], GF[i], GA[i]);
        if (GD[i] >= 0)
            printf("+%d\t %d\n", GD[i], pts[i]);
        else
            printf("%d\t %d\n", GD[i], pts[i]);
    }
    // free
    for (size_t i = 0; i < team_num; i++)
    {
        free(teams[i]);
    }
    Vector_free_ptr((void **)teams);
    Vector_free(win);
    Vector_free(draw);
    Vector_free(lose);
    Vector_free(GF);
    Vector_free(GA);
    return 0;
}