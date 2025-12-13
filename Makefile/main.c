#include "stdio.h"
#include "string.h"
#include "strutils.h"

int main(void)
{
    char str[50]="abcef\0";
    str_reverse(str);
    char str1[50]="ab cd  ef  \0";
   // printf("jello");
   str_trim(str1);
   
    printf("%s\n",str);
    printf("%s\n",str1);
    printf("%d\n",str_to_int("12345"));
    return 0;
}