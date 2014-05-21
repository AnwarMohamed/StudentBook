#include "misc.h"
#include <string.h>
#include <cctype>

void remove_comma(char* str, int i, int len)
{
    for (i=0; i<len; i++) {
        if (str[i]==',') {
            str[i] = ' ';
        }
    }
}

int sub_string(char* s1, char* s2) {
    int len1 = strlen(s1),
    len2 = strlen(s2), i, j;
    if (len2 > len1) return 0;

    for (i=0; i<len1; i++)
        for (j=0; j<len2; j++)
            if (tolower(s1[i+j]) == tolower(s2[j]) && j+1==len2)
                return 1;
    return 0;
}

void trim(char *str)
{
    size_t len = 0;
    char *start = str - 1;
    char *end = NULL;

    if(str == NULL) return;
    if(str[0] == '\0') return;

    len = strlen(str);
    end = str + len;

    while(isspace(*(++start)));
    while(isspace(*(--end)) && end != start);

    if(str + len - 1 != end) *(end + 1) = '\0';
    else if( start != str &&  end == start ) *str = '\0';

    end = str;
    if(start != str)
    {
        while(*start) *end++ = *start++;
        *end = '\0';
    }
}

int is_numeric(char* str)
{
    int i, len = strlen(str);
    if ( len > 0 )
    {
        for(i = 0; i < len; i++)
            if (!isdigit(str[i]))
                return 0;
    }
    return 1;
}

void f_capital(char* str)
{
    int i, len = strlen(str);
    if ( len > 0 )
    {
        str[0] = toupper(str[0]);
        for(i = 1; i < len; i++)
            str[i] = tolower(str[i]);
    }
}

