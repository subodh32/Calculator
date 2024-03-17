#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FUNCTION_NAME 100
#define MAX_FUNCTIONS 100
#define MAX_INPUT 1000

double mode_claculator(char input[MAX_INPUT]);
void mode_help();

int is_number(char a);
int is_function(char a);
int is_operation(char a);

double calc(char inputs[MAX_FUNCTIONS][MAX_FUNCTION_NAME], int *start);
double process_function(char function[MAX_FUNCTION_NAME], double num);


int main()
{
    char input[MAX_INPUT];

    while(strcmp(input,"exit\n") != 0) //while input != "exit"
    {
        fgets(input, 1000, stdin);

        if(strcmp(input,"mode\n") == 0)
        {
            printf("mode\n");
        }
        else if(strcmp(input,"help\n") == 0)
        {
            mode_help();
        }
        else if(strcmp(input,"exit\n") != 0)
        {
            printf("ans: %lf\n", mode_claculator(input) );
        }
    }

    return 0;
}

double mode_claculator(char input[MAX_INPUT])
{
    char filtered[MAX_FUNCTIONS][MAX_FUNCTION_NAME];

    int i = 0, j = 0, index = 0;
    char temp[100];
    while (input[i] != '\0')
    {
        switch (input[i])
        {
        case '+':
            strcpy(filtered[index++], "+");
            break;
        case '-':
            strcpy(filtered[index++], "-");
            break;
        case '*':
            strcpy(filtered[index++], "*");
            break;
        case '/':
            strcpy(filtered[index++], "/");
            break;
        case '(':
            strcpy(filtered[index++], "(");
            break;
        case ')':
            strcpy(filtered[index++], ")");
            break;
        default:
            if (is_number(input[i]))
            {
                j = 0;
                while (is_number(input[i]))
                {
                    temp[j] = input[i];
                    j++;
                    i++;
                }
                i--;
                temp[j] = '\0';
                strcpy(filtered[index], temp);
                index++;
            }

            else if (is_function(input[i]))
            {
                j = 0;
                while (is_function(input[i]))
                {
                    temp[j] = input[i];
                    j++;
                    i++;
                }
                i--;
                temp[j] = '\0';
                strcpy(filtered[index], temp);
                index++;
            }

            break;
        }

        i++;
    }

    filtered[index][0] = '\0';

    /*
        The above code filters the output so that every individual number,operation and
        function is stored in array of strings 'filtered'

        ex: 1+2+sin(1)*4
        will be stored as:
        {1,+,2,sin,(,1,),*,4}
    */

    index = 0;
    // while(filtered[index][0] != '\0')
    // {
    //     printf("%s\n",filtered[index]);
    //     index++;
    // }
    //TODO: remove this comment after debugging is complete

    int start = 0;
    double ans = calc(filtered, &start);

    //printf("ans: %lf", ans);
    return ans;
}

int is_number(char a)
{
    if ((a >= '0' && a <= '9') || a == '.')
    {
        return 1;
    }

    return 0;
}

int is_operation(char a)
{
    if (a == '+' || a == '-' || a == '*' || a == '/')
        return 1;
    return 0;
}

int is_function(char a)
{
    if ((a >= 'a' && a <= 'z'))
    {
        return 1;
    }

    return 0;
}

double calc(char inputs[MAX_FUNCTIONS][MAX_FUNCTION_NAME], int *start)
{
    int i = *start;
    char operation = '\0';
    double sum = 0;

    char element[100], function[100];
    double num;

    while (inputs[i][0] != '\0' && inputs[i][0] != ')')
    {
        strcpy(element, inputs[i]);

        if (element[0] == '(')
        {
            i++;
            num = calc(inputs, &i);
        }

        else if (is_number(element[0]))
        {
            sscanf(element, "%lf", &num);
        }

        else if (is_function(element[0]))
        {
            strcpy(function, element);
            i++;
            strcpy(element, inputs[i]);

            if (is_number(inputs[i][0]))
            {
                sscanf(element, "%lf", &num);
            }
            else if (inputs[i][0] == '(')
            {
                i++;
                num = calc(inputs, &i);
            }

            num = process_function(function, num);
        }

        else if (is_operation(element[0]))
        {
            operation = element[0];
            i++;
            continue;
        }

        if (operation == '\0')
        {
            sum = num;
        }
        else
        {
            switch (operation)
            {
            case '+':
                sum += num;
                break;

            case '-':
                sum -= num;
                break;

            case '*':
                sum *= num;
                break;

            case '/':
                sum = sum / num;
                break;
            }
        }

        i++;
    }

    *start = i;
    return sum;
}

double process_function(char function[MAX_FUNCTION_NAME], double num)
{
    if (strcmp(function, "fact") == 0)
    {
        int fact = 1;
        for(int i = 1; i <= num; i++)
        {
            fact = fact * i;
        }
        return fact;
    }
    return 0;
}

void mode_help()
{
    printf("This calculator computes from left to right\n");
    printf("Things inside prenthesis are computed first\n");
    printf("The following are valid functions:\n");
    printf("sin(x)\n");
    printf("cos(x)\n");
    printf("tan(x)\n");
    printf("fact(x)\n ");
}