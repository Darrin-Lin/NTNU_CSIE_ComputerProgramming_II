#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "myvector.h"
#include "taiko.h"

#define fptf fprintf

double bpm, offset, time;
uint32_t measure_beat, measure_note;
Course taiko_course;

static int8_t read_int_value(int32_t *value);
static int8_t read_double_value(double *value);
static int8_t read_measure_bpmchange(char **input);
static int8_t read_chart(char **input, Chart *chart);

static int8_t read_int_value(int32_t *value)
{
    char *temp;
    char *value_string = strtok(NULL, "\r\n");
    int32_t int_temp = 0;
    int_temp = strtol(value_string, &temp, 10);
    if (int_temp == 0 || *temp != '\0')
        return -1;
    *value = int_temp;
    return 0;
}
static int8_t read_double_value(double *value)
{
    char *temp;
    char *value_string = strtok(NULL, "\r\n");
    double double_temp = 0;
    double_temp = strtod(value_string, &temp);
    if (double_temp == 0 || *temp != '\0')
        return -1;
    *value = double_temp;
    return 0;
}

static int8_t read_measure_bpmchange(char **input)
{
    char *temp;
    char *is_eof = NULL;

    while (!feof(stdin)) // get measure and bpm change
    {

        temp = strtok(*input, " ");
        if (temp == NULL)
        {
            return -1;
        }
        if (strcmp(temp, "#END") == 0)
        {
            return 1;
        }
        else if (strcmp(temp, "#MEASURE") == 0)
        {
            temp = strtok(NULL, "/");
            if (temp == NULL)
            {
                return -1;
            }
            measure_beat = strtol(temp, NULL, 10);
            temp = strtok(NULL, "\r\n");
            if (temp == NULL)
            {
                return -1;
            }
            measure_note = strtol(temp, NULL, 10);
        }
        else if (strcmp(temp, "#BPMCHANGE") == 0)
        {
            temp = strtok(NULL, "\r\n");
            if (temp == NULL)
            {
                return -1;
            }
        }
        is_eof = fgets(*input, 600, stdin);
        if (is_eof == NULL)
        {
            return 2;
        }
        if (strchr(*input, ',') != NULL)
        {
            break;
        }
    }
    return 0;
}
static int8_t read_chart(char **input, Chart *chart)
{
    char *temp;
    char *is_eof = NULL;
    uint64_t measure_size = 0;
    while (!feof(stdin)) // get chart
    {
        // code
        uint32_t chart_size = 0;
        if (*input == "\r\n" || *input == "\n")
        {
            continue;
        }
        *temp = strtok(*input, ",");
        chart_size = strlen(temp);

        /*
        code
        */
        is_eof = fgets(*input, 600, stdin);
        if (is_eof == NULL)
        {
            return 2;
        }
        if (strchr(*input, '#') != NULL)
        {
            break;
        }
    }
    return 0;
}

int main()
{
    char *temp = NULL;
    char input[600];
    char *is_eof = NULL;
    int8_t status = 0;
    time = 0;
    while (!feof(stdin)) // get global value
    {
        is_eof = fgets(input, 600, stdin);
        if (is_eof == NULL)
        {
            return 0;
        }
        // if (input[0] == '\n' || input[0] == '\r')
        // {
        //     break;
        // }
        if (strchr(input, ':') == NULL)
        {
            break;
        }

        temp = strtok(input, ":");
        // fptf(stderr, "%s", temp);
        if (temp == NULL)
        {
            return -1;
        }
        if (strcmp(temp, "BPM") == 0)
        {
            if (read_double_value(&bpm) != 0)
                return -1;
        }

        else if (strcmp(temp, "WAVE") == 0)
        {
            continue;
        }
        else if (strcmp(temp, "OFFSET") == 0)
        {
            if (read_double_value(&offset) != 0)
                return -1;
            continue;
        }

        else
        {
            continue;
        }
    }
    fptf(stderr, "BPM: %lf\n", bpm);
    fptf(stderr, "OFFSET: %lf\n", offset);
    time = -offset;
    while (!feof(stdin)) // get course
    {
        is_eof = fgets(input, 600, stdin);
        if (is_eof == NULL)
        {
            return 0;
        }
        if (strcmp(input, "#START\r\n") == 0)
        {
            break;
        }
        temp = strtok(input, ":");
        if (temp == NULL)
        {
            return -1;
        }
        if (strcmp(temp, "COURSE") == 0)
        {

            char *course_str = strtok(NULL, "\r\n");
            if (strcmp(course_str, "Easy") == 0)
            {
                taiko_course = EASY;
            }
            else if (strcmp(course_str, "Normal") == 0)
            {
                taiko_course = NORMAL;
            }
            else if (strcmp(course_str, "Hard") == 0)
            {
                taiko_course = HARD;
            }
            else if (strcmp(course_str, "Oni") == 0)
            {
                taiko_course = ONI;
            }
            else if (strcmp(course_str, "Edit") == 0)
            {
                taiko_course = EDIT;
            }
            else if (strcmp(course_str, "0") == 0)
            {
                taiko_course = EASY;
            }
            else if (strcmp(course_str, "1") == 0)
            {
                taiko_course = NORMAL;
            }
            else if (strcmp(course_str, "2") == 0)
            {
                taiko_course = HARD;
            }
            else if (strcmp(course_str, "3") == 0)
            {
                taiko_course = ONI;
            }
            else if (strcmp(course_str, "4") == 0)
            {
                taiko_course = EDIT;
            }
            else
            {
                return -1;
            }
        }
    }

    status = read_measure_bpmchange(&input);
    if (status == 2)
    {
        goto generate;
    }
    else if (status == 1)
    {
        goto song_end;
    }
    else if (status == -1)
    {
        return -1;
    }
    // now input is the first note

    fptf(stderr, "COURSE: %d\n", taiko_course);
    fptf(stderr, "MEASURE: %d/%d\n", measure_beat, measure_note);
    fputs(input, stderr);
song_end:

    time = -offset;
generate:
    return 0;
}