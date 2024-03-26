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
    long double time;
    TaikoNote note;
} Hit;
typedef struct _sheet
{
    Hit *hits;
    uint32_t size;
} Sheet;

long double bpm, offset, time_now;
uint32_t measure_beat, measure_note;
Course taiko_course;
Sheet sheets[5];
uint64_t line = 0;
char *buffer;

// static int8_t read_int_value(int32_t *value);
static int8_t read_double_value(long double *value);
static int8_t read_measure_bpmchange(char input[600]);
static int8_t read_chart(char input[600]);
// static int32_t gcd(int32_t a, int32_t b);
// static int32_t lcm(int32_t a, int32_t b);

/*
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
*/
/*
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
*/
static int8_t read_double_value(long double *value)
{
    char *temp;
    char *value_string = strtok(NULL, "\r\n");
    long double double_temp = 0;
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
        if (strstr(input, "#END") != NULL)
        {
            return END_tag;
        }
        else if (strcmp(temp, "#MEASURE") == 0)
        {
            temp = strtok(NULL, "/");
            if (temp == NULL)
            {
                fptf(stderr, "measure error\n");
                return -2;
            }
            measure_beat = strtol(temp, NULL, 10);
            temp = strtok(NULL, "\r\n");
            if (temp == NULL)
            {
                fptf(stderr, "measure error /\n");
                return -2;
            }
            measure_note = strtol(temp, NULL, 10);
        }
        else if (strcmp(temp, "#BPMCHANGE") == 0)
        {
            temp = strtok(NULL, "\r\n");
            if (temp == NULL)
            {
                fptf(stderr, "bpm error\n");
                return -2;
            }
            bpm = strtod(temp, NULL);
        }
        is_eof = fgets(input, 600, stdin);
        line++;
        if (is_eof == NULL)
        {
            return -1;
        }
        if (strstr(input, ",\r\n") != NULL)
        {
            fptf(stderr, "%ld\n", line);
            break;
        }
        else if (input[0] >= '0' && input[0] <= '9')
        {
            fptf(stderr, "%ld\n", line);
            char *temp_buffer = strtok(input, "\r\n");
            if (buffer == NULL)
            {
                buffer = (char *)calloc(strlen(temp_buffer) + 1, sizeof(char));
                if (buffer == NULL)
                {
                    fptf(stderr, "memory error\n");
                    return -1;
                }
                strcpy(buffer, temp_buffer);
                fptf(stderr, "%s\n", buffer);
            }
            else
            {
                char *tmp = NULL;
                tmp = (char *)realloc(buffer, strlen(buffer) + strlen(temp_buffer) + 1);
                if (tmp == NULL)
                {
                    fptf(stderr, "memory error\n");
                    return -1;
                }
                buffer = tmp;
                strncat(buffer, temp_buffer, strlen(temp_buffer));
            }
        }
    }
    return 0;
}
static int8_t read_chart(char input[600])
{
    char *temp;
    // char *is_eof = NULL;
    while (!feof(stdin)) // get chart
    {
        // code
        uint32_t chart_size = 0;
        if (buffer != NULL)
        {
            char *tmp = NULL;
            tmp = (char *)realloc(buffer, strlen(buffer) + strlen(input) + 1);
            if (tmp == NULL)
            {
                fptf(stderr, "memory error\n");
                return -1;
            }
            buffer = tmp;
            strncat(buffer, input, strlen(input));
            for (size_t i = 0; i <= strlen(buffer); i++)
            {
                input[i] = buffer[i];
            }
            free(buffer);
            buffer = NULL;
        }
        if (input[0] == ',')
        {
            time_now += ((60.0 / bpm) * (4.0 / measure_note)) * measure_beat;
        }
        else
        {
            temp = strtok(input, ",");
            chart_size = strlen(temp);
            // fptf(stderr, "%d\n", chart_size);
            // long double length = 0;
            long double duration = 0;
            if (chart_size != 0)
            {
                // length = lcm(chart_size, measure_beat);
                // duration = (60.0 / bpm) * (4.0 / measure_note) * (measure_beat / length);
                duration = ((60.0 / bpm) * (4.0 / measure_note)) * ((long double)measure_beat / chart_size);
                for (uint32_t i = 0; i < chart_size; i++)
                {
                    if (temp[i] == '0' || (temp[i] - '0' > 4 && temp[i] - '9' <= 0))
                    {
                        time_now += duration;
                    }
                    else if (temp[i] - '0' > 0 && temp[i] - '0' < 5)
                    {
                        sheets[taiko_course].size++;
                        sheets[taiko_course].hits = (Hit *)realloc(sheets[taiko_course].hits, (sheets[taiko_course].size) * sizeof(Hit));
                        sheets[taiko_course].hits[sheets[taiko_course].size - 1].time = time_now;
                        sheets[taiko_course].hits[sheets[taiko_course].size - 1].note = temp[i] - '0';
                        // time_now += duration * (length / chart_size);
                        time_now += duration;
                    }
                }
            }
            else
            {
                time_now += ((60.0 / bpm) * (4.0 / measure_note)) * measure_beat;
            }
        }
    // is_eof = fgets(input, 600, stdin);
    getline:
        fgets(input, 600, stdin);
        line++;
        // if (is_eof == NULL)
        // {
        //     return -1;
        // }
        // for (int32_t i = 0; i < strlen(input); i++)
        // fptf(stderr, "%d", input[i]);
        // fptf(stderr,"\n");
        // fptf(stderr,"%d",strcmp(input,"\r\n"));
        if (strchr(input, '#') != NULL)
        {
            fptf(stderr, "-%ld\n", line);
            if (strstr(input, "#END") != NULL)
            {
                fptf(stderr, "#END-%ld\n", line);
                return END_tag;
            }
            return 0;
        }
        if (input[0] >= '0' && input[0] <= '9' && strchr(input, ',') == NULL)
        {
            fptf(stderr, "%ld\n", line);
            char *temp_buffer = strtok(input, "\r\n");
            if (buffer == NULL)
            {
                buffer = (char *)calloc(strlen(temp_buffer) + 1, sizeof(char));
                if (buffer == NULL)
                {
                    fptf(stderr, "memory error\n");
                    return -1;
                }
                strcpy(buffer, temp_buffer);
            }
            else
            {
                char *tmp = NULL;
                tmp = (char *)realloc(buffer, strlen(buffer) + strlen(temp_buffer) + 1);
                if (tmp == NULL)
                {
                    fptf(stderr, "memory error\n");
                    return -1;
                }
                buffer = tmp;
                strncat(buffer, temp_buffer, strlen(temp_buffer));
            }
            goto getline;
        }
        if (strcmp(input, "\r\n") == 0 || strcmp(input, "\n") == 0 || strcmp(input, "\r") == 0)
        {
            while (!feof(stdin) && (strcmp(input, "\r\n") == 0 || strcmp(input, "\n") == 0 || strcmp(input, "\r") == 0))
            {
                // is_eof = fgets(input, 600, stdin);
                fgets(input, 600, stdin);
                line++;
                // if (is_eof == NULL)
                // {
                //     return -1;
                // }
                if (strchr(input, '#') != NULL)
                {
                    if (strstr(input, "#END") != NULL)
                        return 1;
                    return 0;
                }
            }
        }
    }
    return 0;
}

int main()
{
    char *temp = NULL;
    char input[600];
    // char *is_eof = NULL;
    int8_t status = 0;
    bpm = 0;
    time_now = 0;
    offset = 0;
    measure_beat = 4;
    measure_note = 4;
    taiko_course = 0;
    buffer = NULL;
    // char order[10];
    int8_t order_count = 0;
    for (int32_t i = 0; i < 5; i++)
    {
        sheets[i].hits = NULL;
        sheets[i].size = 0;
    }
    while (!feof(stdin)) // get global value
    {
        // is_eof = fgets(input, 600, stdin);
        fgets(input, 600, stdin);
        line++;
        // if (is_eof == NULL)
        // {
        //     fptf(stderr,"EOF");
        //     return 0;
        // }
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
            fptf(stderr, ":error\n");
            return -1;
        }
        if (strcmp(temp, "BPM") == 0)
        {
            if (read_double_value(&bpm) != 0)
            {
                fptf(stderr, "bpm error\n");
                return -1;
            }
        }

        else if (strcmp(temp, "WAVE") == 0)
        {
            continue;
        }
        else if (strcmp(temp, "OFFSET") == 0)
        {
            if (read_double_value(&offset) != 0)
            {
                fptf(stderr, "offset error\n");
                return -1;
            }
            continue;
        }

        else
        {
            continue;
        }
    }
    fptf(stderr, "BPM: %Lf\n", bpm);
    fptf(stderr, "OFFSET: %Lf\n", offset);
    long double G_bpm = bpm;
    time_now = -offset;
    fptf(stderr, "%Lf\n", time_now);
    fptf(stderr, "line:%ld\n", line);
    while (!feof(stdin))
    {
        bpm = G_bpm;
        // fptf(stderr, "%d\n", order_count);
        while (!feof(stdin)) // get course
        {
            // is_eof = fgets(input, 600, stdin);
            fgets(input, 600, stdin);
            line++;
            // if (is_eof == NULL)
            // {
            //     return 0;
            // }
            if (strstr(input, "#START") != NULL)
            {
                fptf(stderr, "#START:%ld\n", line);
                break;
            }
            temp = strtok(input, ":");
            if (temp == NULL)
            {
                fptf(stderr, ":error\n");
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
                    fptf(stderr, "course error\n");
                    return -1;
                }
            }
        }
        // order[order_count] = taiko_course + '0';
        order_count++;
        fptf(stderr, "courese:%d\n", taiko_course);
        while (!feof(stdin))
        {
            // fptf(stderr, "A");
            status = read_measure_bpmchange(input);
            if (status == EOF)
            {
                goto generate;
            }
            else if (status == END_tag)
            {
                // fptf(stderr, "END\n");
                break;

                // goto song_end;
            }
            else if (status == -2)
            {
                fptf(stderr, "-2error\n");
                return -1;
            }
            // fptf(stderr, "beat:%d, note%d\n", measure_beat, measure_note);
            // now input is the first note
            // fptf(stderr,"--%s",input);
            status = read_chart(input);
            // fptf(stderr, "a");
            if (status == EOF)
            {
                goto generate;
            }
            else if (status == -2)
            {
                fptf(stderr, "-2error\n");
                return -1;
            }
            else if (status == END_tag)
            {
                break;
                // goto song_end;
            }
        }
        // song_end:
        time_now = -offset;
    }
    fptf(stderr, "No error\n");
generate:
    printf("{\n \"data\": [\n");
    for (int32_t i = 4; i >= 0; i--)
    {
        if (sheets[i].size == 0)
        {
            continue;
        }
        printf("{\n   \"course\": %d,\n", i);
        printf("  \"chart\": [\n");
        for (uint32_t j = 0; j < sheets[i].size - 1; j++)
        {
            printf("    [%d, %Lf],\n", sheets[i].hits[j].note, sheets[i].hits[j].time);
            // fptf(stderr,"%Lf\n",sheets[i].hits[j].time);
        }
        printf("    [%d, %Lf]\n", sheets[i].hits[sheets[i].size - 1].note, sheets[i].hits[sheets[i].size - 1].time);

        printf("  ]\n  }");
        order_count--;
        if (order_count != 0 && i != 0)
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