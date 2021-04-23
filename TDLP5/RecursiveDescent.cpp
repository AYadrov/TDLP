#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Terminals */
#define variance       0
#define equal          1
#define minus          2
#define number         3
#define zero           4
#define sign           5
#define left_bracket   6
#define right_bracket  7
#define decimal_number 8
#define octal_number   9
#define delimiter     10


/* EOP */
#define EOP           11

#define UNDEF         12

/* Lexeme class */
int lexeme = 0;

/* Non terminals */
void S(), R(), A(), B(), J(), P();

/* Checking binary of string which contains number */
bool IsBinary(char* s);

/* Checking octal of string which contains number */
bool IsOctal(char* s);

/* Reject input line */
void error();

/* Look ahead lexeme */
int get_token();

char g_inputBuffer[1024] = "";
char* g_prog = NULL;

int main(int argc, char* argv[])
{
    if (1 == argc)
    {
        printf("Enter your input line (delimiter betweens expressions = ';'): ");
        gets(g_inputBuffer);
    }
    else
    {
        strcpy(g_inputBuffer, argv[1]);
    }

    g_prog = (char*)g_inputBuffer;

    lexeme = get_token();
    S();
    lexeme = get_token();
    if (lexeme == EOP)
        printf("Accept.\n");
    else
        printf("Reject.\n");
}

void S()
{
    if (lexeme != variance)
        error();

    lexeme = get_token();
    R();

    if (lexeme != delimiter)
       error();

    lexeme = get_token();

    if (lexeme != EOP)
        S();
}

void R()
{
    if (lexeme != equal)
    {
        error();
    }
    lexeme = get_token();
    if (lexeme == minus)
        A();
    else
        B();
}

void A()
{
    if (lexeme != minus)
    {
        error();
    }
    lexeme = get_token();
    B();
}

void B()
{
    if ((lexeme == number) || (lexeme == variance) ||
            (lexeme == decimal_number) || (lexeme == octal_number))
    {
        lexeme = get_token();
        if (lexeme != EOP && (lexeme == sign || lexeme == minus))
        {
            J();
        }
    }
    else if (lexeme == left_bracket)
    {
        lexeme = get_token();
        if (lexeme == minus)
            A();
        else
            B();
        P();
    }
    else
    {
        error();
    }
}

void J()
{
    if (lexeme != sign && lexeme != minus)
    {
        error();
    }
    lexeme = get_token();
    B();
}

void P()
{
    if (lexeme == sign || lexeme == minus)
    {
        lexeme = get_token();
        B();
        P();
    }
    else if (lexeme == right_bracket)
    {
        lexeme = get_token();
        if (lexeme != EOP && (lexeme == sign || lexeme == minus))
            J();
    }
    else
    {
        printf("%d", lexeme);
        error();
    }
}

int get_token()
{
    char token[132] = "";
    char* tok = token;

    /* Skip spaces */
    while (isspace(*g_prog))
        ++g_prog;

     /*End line marker*/
    if (*g_prog == NULL)
         return EOP;

    /* printf(g_prog);
    printf("\n"); */
    /* Keywords */
    if (isalpha(*g_prog)) // Чтение переменной из 1-2 символов
    {
        ++g_prog;
        if (isalpha(*g_prog))
        {
            ++g_prog;
        }
        return variance;
    }
    else if(*g_prog == '=')  // Чтение знака равенства
    {
        ++g_prog;
        return equal;
    }
    else if(*g_prog == '-')  // Чтение унарного минуса
    {
        ++g_prog;
        return minus;
    }
    else if (isdigit(*g_prog))  // Чтение любого числа
    {

        if (*g_prog != '0')
        {
            while (isdigit(*g_prog))
            {
                *tok = *g_prog;
                ++tok;
                ++g_prog;
            }
        }
        else
        {
            *tok = *g_prog;
            ++tok;
            ++g_prog;
        }
        *tok = '\0';
        if (0 == strcmp(token, "0"))
            return number;
        if (IsBinary(tok))
            return decimal_number;
        if (IsOctal(tok))
            return octal_number;
        else
            return number;
    }
    else if ((*g_prog == '+') || (*g_prog == '*') || (*g_prog == '%') || (*g_prog == '/'))
    {
        ++g_prog;
        return sign;
    }
    else if (*g_prog == '(')
    {
        ++g_prog;
        return left_bracket;
    }
    else if (*g_prog == ')')
    {
        ++g_prog;
        return right_bracket;
    }
    else if (*g_prog == ';')
    {
        ++g_prog;
        return delimiter;
    }
    return UNDEF;
}

void error()
{
    printf("Reject.\n");
    exit(EXIT_FAILURE);
}

bool IsBinary( char* s )
{
   bool bIsBinary = true;
   int i = 0;
   while(s[i] != '\0')
   {
       if (s[i] != '0' && s[i] != '1')
           bIsBinary = false;
       i++;
   }
   return bIsBinary;
}

bool IsOctal( char* s )
{
   bool IsOctal = true;
   int i = 0;
   while(s[i] != '\0')
   {
       if (s[i] != '0' && s[i] != '1' &&
               s[i] != '2' && s[i] != '3' &&
               s[i] != '4' && s[i] != '5' &&
               s[i] != '6' && s[i] != '7')
           IsOctal = false;
       i++;
   }
   return IsOctal;
}
