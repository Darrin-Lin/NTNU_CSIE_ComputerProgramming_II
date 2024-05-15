#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    //top -o -PID -b -n 1 | awk 'BEGIN {p=0} {p=1; getline} {if (p) print $1" "$12" "$8" "$9" "$7*1024}'     | head -n -2 | tail -n +4
    system("top -o -PID -b -n 1 | awk 'BEGIN {p=0} {p=1; getline} {if (p) print $1\" \"$12\" \"$8\" \"$9\" \"$7*1024}' | head -n -2 | tail -n +4");
    return 0;
}