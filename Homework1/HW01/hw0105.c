#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "myvector.h"
#include "taiko.h"

#define fptf fprintf
#define END_tag 1

typedef struct _hit
{
    double time;
    TaikoNote note;
} Hit;
typedef struct _sheet
{
    Hit *hits;
    uint32_t size;
} Sheet;

double bpm, offset, time;
uint32_t measure_beat, measure_note;
Course taiko_course;
Sheet sheets[5];

static int8_t read_int_value(int32_t *value);
static int8_t read_double_value(double *value);
static int8_t read_measure_bpmchange(char input[600]);
static int8_t read_chart(char input[600]);
static int32_t gcd(int32_t a, int32_t b);
static int32_t lcm(int32_t a, int32_t b);

static int32_t gcd(int32_t a, int32_t b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}
static int32_t lcm(int32_t a, int32_t b)
{
    return a * b / gcd(a, b);
}
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

static int8_t read_measure_bpmchange(char input[600])
{
    char *temp;
    char *is_eof = NULL;

    while (!feof(stdin)) // get measure and bpm change
    {

        temp = strtok(input, " ");
        if (temp == NULL)
        {
            return -2;
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
                return -2;
            }
            measure_beat = strtol(temp, NULL, 10);
            temp = strtok(NULL, "\r\n");
            if (temp == NULL)
            {
                return -2;
            }
            measure_note = strtol(temp, NULL, 10);
        }
        else if (strcmp(temp, "#BPMCHANGE") == 0)
        {
            temp = strtok(NULL, "\r\n");
            if (temp == NULL)
            {
                return -2;
            }
        }
        is_eof = fgets(input, 600, stdin);
        if (is_eof == NULL)
        {
            return -1;
        }
        if (strchr(input, ',') != NULL)
        {
            break;
        }
    }
    return 0;
}
static int8_t read_chart(char input[600])
{
    char *temp;
    char *is_eof = NULL;
    while (!feof(stdin)) // get chart
    {
        // code
        uint32_t chart_size = 0;
        if (strcmp(input, "\r\n") == 0 || strcmp(input, "\n") == 0)
        {
            continue;
        }
        temp = strtok(input, ",");
        chart_size = strlen(temp);

        double length = 0;
        double duration = 0;
        if (chart_size != 0)
        {
            length = lcm(chart_size, measure_beat);
            duration = (60 / bpm) * (4 / measure_note) * (measure_beat / length);
            for (uint32_t i = 0; i < chart_size; i++)
            {
                if (temp[i] == '0')
                {
                    time += duration;
                }
                else
                {
                    sheets[taiko_course].size++;
                    sheets[taiko_course].hits = (Hit *)realloc(sheets[taiko_course].hits, (sheets[taiko_course].size) * sizeof(Hit));
                    sheets[taiko_course].hits[sheets[taiko_course].size - 1].time = time;
                    sheets[taiko_course].hits[sheets[taiko_course].size - 1].note = temp[i] - '0';
                    time += duration * (length / chart_size);
                }
            }
        }
        else
        {
            time += ((60 / bpm) * (4 / measure_note)) * measure_beat;
        }
        is_eof = fgets(input, 600, stdin);
        if (is_eof == NULL)
        {
            return -1;
        }
        if (strchr(input, '#') != NULL)
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
    char oreder[10];
    int8_t oreder_count = 0;
    for (int32_t i = 0; i < 5; i++)
    {
        sheets[i].hits = NULL;
        sheets[i].size = 0;
    }
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
    while (!feof(stdin))
    {
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
        oreder[oreder_count] = taiko_course + '0';
        oreder_count++;

        while (1)
        {
            status = read_measure_bpmchange(input);
            if (status == EOF)
            {
                goto generate;
            }
            else if (status == END_tag)
            {
                break;
                // goto song_end;
            }
            else if (status == -2)
            {
                return -1;
            }
            // now input is the first note
            status = read_chart(input);
            if (status == EOF)
            {
                goto generate;
            }
            else if (status == -2)
            {
                return -1;
            }
            else if (status == END_tag)
            {
                break;
                // goto song_end;
            }
        }
        // song_end:
        time = -offset;
    }

generate:
    printf("{\n \"data\": [\n");
    for (int32_t i = 0; i < 5; i++)
    {
        if (sheets[i].size == 0)
        {
            continue;
        }
        printf("{\n   \"course\": %d,\n", i);
        printf("  chart: [\n");
        for (uint32_t j = 0; j < sheets[i].size; j++)
        {
            printf("    [%d, %lf],\n", sheets[i].hits[j].note, sheets[i].hits[j].time);
        }
        printf("  ]\n  }");
        oreder_count--;
        if (oreder_count != 0)
        {
            printf(",\n");
        }
        else
        {
            printf("\n");
        }
    }
    printf("]\n}");
    return 0;
}