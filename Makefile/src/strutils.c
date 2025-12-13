#include "stdio.h"
#include "string.h"
#include "strutils.h"
#include "stdint.h"


int str_reverse(char str[])
{
    int length =strlen(str)-1;
    for(int i=0;i<=length/2;i++)
    {
        char temp=str[i];
        str[i]=str[length-i];
        str[length-i]=temp;
    }
    return length;
} // abc a d 
int str_trim(char str[])
{
    int length =strlen(str)-1;
    int j=0;
    for( int i=0;i<=length;i++)
    {
        if( str[i] !=' ' && str[i]!='\t' && str[i]!='\n')
        {
            str[j++]=str[i];
        }
    }
    str[j]='\0';
    return j;
}
int str_to_int(char str[])
{
    int length =strlen(str)-1;
    int char_to_int=0;
    for( int i =0;i<=length;i++)
    {
        int temp=(str[i]-'0');
        char_to_int= (char_to_int*10) +temp;
    }

    return char_to_int;
}




