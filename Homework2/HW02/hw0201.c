#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define fptf fprintf
#define DEBUG 0

int32_t primary_colour = 0;
int64_t time_shift = 0;
double speed = 0;

typedef struct _Stime
{
    int64_t start;
    int64_t end;
} Stime;

static int8_t mystrcpy(char *dest, const char *src, int32_t len, char stop);

int main()
{
    char file_name[100];
    printf("Please enter the file name: ");
    fgets(file_name, 100, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        return -1;
    }
    char line[1000];
    printf("Time Shift ( -10 ~ 10 ): ");
    scanf("%ld", &time_shift);
    if (time_shift < -10 || time_shift > 10)
    {
        goto err_close;
    }
    time_shift *= 1000000;
    if (time_shift < 0)
    {
        usleep(-time_shift);
    }

    printf("Speed (0.25,0.5,0.75,1,1.25,1.5,1.75,2): ");
    scanf("%lf", &speed);
    if (fabs(speed - 0.25) > 1e-6 && fabs(speed - 0.5) > 1e-6 && fabs(speed - 0.75) > 1e-6 && fabs(speed - 1) > 1e-6 && fabs(speed - 1.25) > 1e-6 && fabs(speed - 1.5) > 1e-6 && fabs(speed - 1.75) > 1e-6 && fabs(speed - 2) > 1e-6)
    {
        goto err_close;
    }
    int8_t style_check = 0;
    while (!feof(file))
    {
        fgets(line, 1000, file);
        if (strstr(line, "Style:") != NULL)
        {
            style_check = 1;
            char *token = NULL;
            token = strtok(line, ",");
            for (int i = 0; i < 3; i++)
            {
                token = strtok(NULL, ",");
                if (token == NULL)
                {
                    goto err_close;
                }
            }
            if (strchr(token, '&') != NULL)
            {
                token = strtok(token, "H");
                if (token == NULL)
                {
                    goto err_close;
                }
                token = strtok(NULL, "H");
                if (token == NULL)
                {
                    goto err_close;
                }
                primary_colour = strtol(token, NULL, 16);
                if (DEBUG)
                {
                    fptf(stderr, "Primary Colour: %d\n", primary_colour);
                }
            }

            break;
        }
    }
    if (style_check == 0)
    {
        goto err_close;
    }
    Stime last = {0, 0};
    Stime now = {0, 0};
    // Dialogue: 0,0:00:25.06,0:00:28.86,Default,,0,0,0,,... Three minutes.
    // ansi
    printf("\e[38;2;%d;0;0m", primary_colour);
    while (!feof(file))
    {

        char time_start[20] = {0};
        char time_end[20] = {0};
        char santance[1000] = {0};
        fgets(line, 1000, file);
        if (strstr(line, "Dialogue:") == NULL)
        {
            continue;
        }
        char *position = NULL;
        position = strstr(line, ",");
        if (position == NULL)
            goto err_close;
        position++;
        mystrcpy(time_start, position, 20, ',');
        position = strstr(position, ",");
        if (position == NULL)
            goto err_close;
        position++;
        mystrcpy(time_end, position, 20, ',');
        for (int32_t i = 0; i < 7; i++)
        {
            position = strstr(position, ",");
            if (position == NULL)
                goto err_close;
            position++;
        }
        mystrcpy(santance, position, 1000, '\n');
        if (DEBUG)
        {
            fptf(stderr, "time_start: %s\n", time_start);
            fptf(stderr, "time_end: %s\n", time_end);
            fptf(stderr, "santance: %s\n", santance);
        }
        int32_t hour = 0, min = 0;
        double sec = 0;
        sscanf(time_start, "%d:%d:%lf", &hour, &min, &sec);
        if (DEBUG)
        {
            fptf(stderr, "hour: %d, min: %d, sec: %lf\n", hour, min, sec);
        }
        now.start = hour * 3600000000 + min * 60000000 + sec * 1000000;
        sscanf(time_end, "%d:%d:%lf", &hour, &min, &sec);
        if (DEBUG)
        {
            fptf(stderr, "hour: %d, min: %d, sec: %lf\n", hour, min, sec);
        }
        now.end = hour * 3600000000 + min * 60000000 + sec * 1000000;
        if (time_shift > now.end)
        {
            last = now;
            continue;
        }
        if (time_shift > last.end && time_shift < now.start)
        {
            usleep((now.start - time_shift) / speed);
            printf("%s\n", santance);
        }
        else if (time_shift <= now.start)
        {
            usleep((now.start - last.end) / speed);
            printf("%s\n", santance);
        }
        if (time_shift > now.start && time_shift < now.end)
        {
            usleep((now.end - time_shift) / speed);
            system("clear");
        }
        else if (time_shift <= now.end)
        {
            usleep((now.end - now.start) / speed);
            system("clear");
        }

        last = now;
        system("clear");
    }

    return 0;
err_close:
    fclose(file);
    return -1;
}
static int8_t mystrcpy(char *dest, const char *src, int32_t len, char stop)
{
    while (*src != '\0' && *src != stop && len > 0)
    {
        *dest = *src;
        dest++;
        src++;
        len--;
    }
    *dest = '\0';
    return 0;
}