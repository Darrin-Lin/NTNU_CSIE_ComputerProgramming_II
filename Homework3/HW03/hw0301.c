/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
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
#include "cJSON/cJSON.h"

#define DEBUG 0

int32_t to_Upper(int32_t c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c ^ ' ';
    }
    return c;
}

int main(int argc, char *argv[])
{
    FILE *fd = fopen("bible.txt", "r");
    if (fd == NULL)
    {
        perror("fopen");
        return 1;
    }
    char bible[1024];
    FILE *tmp_write = fopen("tmp_write.txt", "w");
    // find the string in the JSON object
    char string[1024];
    printf("Please enter the search target: ");
    fgets(string, 1024, stdin);
    string[strlen(string) - 1] = '\0';
    for (int32_t i = 0; i < strlen(string); i++)
    {
        string[i] = to_Upper(string[i]);
    }
    int32_t found_times = 0;
    while (!feof(fd))
    {

        fgets(bible, 1024, fd);
        cJSON *bible_json = cJSON_Parse(bible);
        if (bible_json == NULL)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL)
            {
                fprintf(stderr, "Error before: %s\n", error_ptr);
            }
            return 1;
        }
        cJSON *book_id = NULL;
        cJSON *chapter = NULL;
        cJSON *verse = NULL;
        cJSON *text = NULL;
        text = cJSON_GetObjectItemCaseSensitive(bible_json, "text");
        book_id = cJSON_GetObjectItemCaseSensitive(bible_json, "book_id");
        chapter = cJSON_GetObjectItemCaseSensitive(bible_json, "chapter");
        verse = cJSON_GetObjectItemCaseSensitive(bible_json, "verse");
        char tmp_text[1024];
        strcpy(tmp_text, text->valuestring);
        for (int32_t i = 0; i < strlen(tmp_text); i++)
        {
            tmp_text[i] = to_Upper(tmp_text[i]);
        }
        if (DEBUG)
        {
            fprintf(stderr, "tmp_text: %s\n", tmp_text);
            fprintf(stderr, "string: %s\n", string);
            fprintf(stderr, "%s\n", strstr(tmp_text, string));
        }
        if (strstr(tmp_text, string) != NULL)
        {
            found_times++;
            fprintf(tmp_write, "%d. %s %d:%d %s\n", found_times, book_id->valuestring, chapter->valueint, verse->valueint, text->valuestring);
        }
        // free the cJSON object
        cJSON_Delete(bible_json);
    }
    fclose(fd);
    fclose(tmp_write);
    FILE *tmp_read = fopen("tmp_write.txt", "r");
    printf("Found %d time(s)\n", found_times);
    while (!feof(tmp_read))
    {
        fgets(bible, 1024, tmp_read);
        if (!feof(tmp_read))
            printf("%s", bible);
    }
    fclose(tmp_read);
    remove("tmp_write.txt");

    return 0;
}
