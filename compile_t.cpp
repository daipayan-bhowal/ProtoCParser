#include "compile_t.h"
#include "CLexer.h"
#include "bool_t.h"
#include <stdlib.h>
bool_t isAlpha(char c)
{
    if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_'))
        return True;

    return False;
}
bool_t isNumeric(char c)
{
    if ('0' <= c && c <= '9')
        return True;

    return False;
}
bool_t isAlphanumeric(char c)
{
    if (isAlpha(c) || isNumeric(c))
        return True;

    return False;
}
void white_spaces(string_t s, int pos)
{
    int i = pos;
    while (s->str[i] == ' ' || s->str[i] == '\v' || s->str[i] == '\t')
    {
        i++;
    }

}

int is_id(string_t s)
{
    int i = 0;
    int current_pos = 0;
    char* Buffer = s->str;
    if ('0' <= Buffer[i] && Buffer[i] <= '9')
    {
        printf("Identifier cannot start with digit  \n");
        exit(0);
    }
    current_pos = i;

    while (isAlphanumeric(Buffer[i]) || Buffer[i] == '_')
    {
        i++;
    }
    return i; // if > 0 then ID , if ==0 then it's not ID
}

int is_constant(string_t s, int* type)
{
    int i = 0;
    int digit_start = 0;
    char* Buffer = s->str;
    if (Buffer[i] == '0' && (Buffer[i + 1] == 'x' || Buffer[i + 1] == 'X'))
    {
        i += 2;
        while (isAlphanumeric(Buffer[i]))
        {
            if ((Buffer[i] > 'f' && Buffer[i] < 'z') || (Buffer[i] > 'F' && Buffer[i] < 'Z'))
            {
                printf("error: Wrong hexadecimal value !\n");
                exit(0);
            }
            i++;
        }
        *type = HEXADECI_CONST;
    }
    else if (Buffer[i] == '0')
    {
        i++;
        while (isNumeric(Buffer[i]))
        {
            i++;
        }

        *type = OCTAL_CONST;
    }
    else if (((Buffer[i] == '+' || Buffer[i] == '-') && ('1' <= Buffer[i + 1] && Buffer[i + 1] <= '9')) || ('1' <= Buffer[i] && Buffer[i] <= '9'))
    {
        digit_start = i;
        i++;
        while (isNumeric(Buffer[i]))
        {
            i++;
        }

        if (Buffer[digit_start] == '-' && (Buffer[i] == 'u' || Buffer[i] == 'U'))
        {
            printf("error: Not an unsigned number!\n");
            exit(0);
        }
        else if (Buffer[i] == 'u' || Buffer[i] == 'U')
        {
            *type = USIGN_INT_CONST;
        }
        else if (Buffer[i] == 'l' || Buffer[i] == 'L')
        {
            *type = LONG_INT_CONST;
        }
        else if (Buffer[i] == '.')
        {
            i++;
            while (isNumeric(Buffer[i]))
                i++;
            if (Buffer[i] == 'f' || Buffer[i] == 'F')
            {
                i++;
                *type = F_CONST;
            }
            else if (Buffer[i] == 'l' || Buffer[i] == 'L')
            {
                i++;
                *type = LONG_DOUBLE_CONST;
            }
            else if (Buffer[i] == 'e' || Buffer[i] == 'E')
            {
                i++;
                if (Buffer[i] == '-')
                    i++;
                while (isNumeric(Buffer[i]))
                    i++;
                *type = E_F_CONST;
            }
            else if (Buffer[i] == ';' || Buffer[i] == ' ' || Buffer[i] == '\v' || Buffer[i] == '\t')
            {
                i++;
                *type = F_CONST;
            }

        }
        else if (Buffer[i] == ';' || Buffer[i] == ' ' || Buffer[i] == '\v' || Buffer[i] == '\t')
            *type = INT_CONST;

    }
    else if (Buffer[i] == '\'')
    {
        i++;
        if (isAlpha(Buffer[i]) || isNumeric(Buffer[i]))
        {
            i++;
            if (Buffer[i] == '\'')
            {
                i++;
                *type = CHAR_CONST;
                return i;
            }
        }
        else
        {
            switch (Buffer[i])
            {
            case '\\':
            {
                i++;
                switch (Buffer[i])
                {
                case 'n':
                case 't':
                case 'b':
                case 'r':
                case 'f':
                case 'a':
                case '\\':
                case '\?':
                case '\"':
                case '\'':
                {
                    i++;
                    if (Buffer[i] == '\'')
                    {
                        i++;
                        *type = ESCAPE_SEQ_CONST;
                        return i;
                    }
                    else
                    {
                        printf("error: wrong escape sequence char:%d !", Buffer[i]);
                        exit(0);
                    }
                    break;
                }
                }
                break;
            }
            }  /* end of switch */
        }



    }
    return i;
}

int is_string(string_t s)
{
    int i = 0;
    char* Buffer = s->str;
    if (Buffer[i] == '\"')
    {
        i++;
        while (Buffer[i] != '\"' && Buffer[i] != '\n')
        {
            i++;
        }
        if (Buffer[i] == '\"')
            i++;
        else if (Buffer[i] == '\n')
        {
            printf("error: String ending couldn't found\n");
            exit(0);
        }
    }
    return i;
}

long ConvertHextoDec(string_t hexval)
{
    //char arr[64];
    long number;
    //StrCopy(&arr, current_ptr, offset);

    //number = (long long)strtol(hexstring, NULL, 16);
    number = (long)strtol(hexval->str, NULL, 0);

    return number;
}
long ConvertOcttoDec(string_t octval)
{
    //char arr[64];
    long number;
    //StrCopy(&arr, current_ptr, offset);

    //number = (long long)strtol(hexstring, NULL, 16);
    number = (long)strtol(octval->str, NULL, 8);

    return number;
}


long ConvertStrtoDec(string_t str)
{
    long dec;
    //char arr[64];
    //StrCopy(&arr, current_ptr, offset);

    dec = atol(str->str);

    return dec;
}
double ConvertStrtoDouble(string_t str)
{
    double fval;
    //char arr[64];
    //StrCopy(&arr, current_ptr, offset);

    fval = atof(str->str);

    return fval;
}
#define ConvertExpoStrtoDouble(str) ConvertStrtoDouble(str)
char ConvertStrtoASCII(string_t str)
{
    char c = str->str[0];
    return c - '0';
}

/*void StrCopy(char** arr, int current_ptr, int offset)
{
    int i;
    for (i = 0; i < (offset - current_ptr); i++)
        *arr[i] = Buffer[i + current_ptr];
    *arr[i] = '\0';
} */