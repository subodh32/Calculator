#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_FUNCTION_NAME 100
#define MAX_FUNCTIONS 100
#define MAX_INPUT 1000

#define PRECISION 0.0001
#define PI 3.14159265359

struct input
{
    char unfiltered[MAX_INPUT];
    char filtered[MAX_FUNCTIONS][MAX_FUNCTION_NAME];
};

double mode_claculator(struct input *inputs);
double mode_integration(struct input *inputs);
void mode_help();

int is_number(char a);
int is_function(char a);
int is_operation(char a);

double calc(char inputs[MAX_FUNCTIONS][MAX_FUNCTION_NAME], int *start);
double process_function(char function[MAX_FUNCTION_NAME], double num);

void filter_inputs(struct input *inputs);

int main()
{
    struct input inputs;

    while (strcmp(inputs.unfiltered, "exit\n") != 0) // while input != "exit"
    {
        fgets(inputs.unfiltered, MAX_INPUT, stdin);

        if (strcmp(inputs.unfiltered, "integration\n") == 0 || strcmp(inputs.unfiltered, "int\n") == 0)
        {
            printf("area = %lf\n", mode_integration(&inputs));
        }
        else if (strcmp(inputs.unfiltered, "help\n") == 0)
        {
            mode_help();
        }
        else if (strcmp(inputs.unfiltered, "exit\n") != 0)
        {
            printf("ans: %lf\n", mode_claculator(&inputs));
        }
    }

    return 0;
}

void filter_inputs(struct input *inputs)
{
    int i = 0, j = 0, index = 0;
    char temp[100];
    while (inputs->unfiltered[i] != '\0')
    {
        switch (inputs->unfiltered[i])
        {
        case '+':
            strcpy(inputs->filtered[index++], "+");
            break;
        case '-':
            strcpy(inputs->filtered[index++], "-");
            break;
        case '*':
            strcpy(inputs->filtered[index++], "*");
            break;
        case '/':
            strcpy(inputs->filtered[index++], "/");
            break;
        case '(':
            strcpy(inputs->filtered[index++], "(");
            break;
        case ')':
            strcpy(inputs->filtered[index++], ")");
            break;
        default:
            if (is_number(inputs->unfiltered[i]))
            {
                j = 0;
                while (is_number(inputs->unfiltered[i]))
                {
                    temp[j] = inputs->unfiltered[i];
                    j++;
                    i++;
                }
                i--;
                temp[j] = '\0';
                strcpy(inputs->filtered[index], temp);
                index++;
            }

            else if (is_function(inputs->unfiltered[i]))
            {
                j = 0;
                while (is_function(inputs->unfiltered[i]))
                {
                    temp[j] = inputs->unfiltered[i];
                    j++;
                    i++;
                }
                i--;
                temp[j] = '\0';
                strcpy(inputs->filtered[index], temp);
                index++;
            }

            break;
        }

        i++;
    }

    inputs->filtered[index][0] = '\0';

    /*
        The above code filters the output so that every individual number,operation and
        function is stored in array of strings 'filtered'

        ex: 1+2+sin(1)*4
        will be stored as:
        {1,+,2,sin,(,1,),*,4}
    */
}

double mode_claculator(struct input *inputs)
{
    filter_inputs(inputs);

    int start = 0;
    double ans = calc(inputs->filtered, &start);
    return ans;
}

double mode_integration(struct input *inputs)
{
    double ans = 0;
    double l_limit, u_limit;
    printf("enter limits in ' lower,upper ' format: ");
    scanf("%lf,%lf", &l_limit, &u_limit);

    fflush(stdin);

    printf("enter function: ");
    fgets(inputs->unfiltered, MAX_INPUT, stdin);
    filter_inputs(inputs);

    char filtered_cpy[MAX_FUNCTIONS][MAX_FUNCTION_NAME];
    int i,start;
    char double_to_str[MAX_FUNCTION_NAME];

    while (inputs->filtered[i][0] != '\0')
    {
        strcpy(filtered_cpy[i], inputs->filtered[i]);
        i++;
    }
    filtered_cpy[i][0] = '\0';

    ans = 0;
    for (double x = l_limit; x < u_limit; x += PRECISION)
    {
        i = 0;
        while (inputs->filtered[i][0] != '\0')
        {
            sprintf(double_to_str, "%lf", x);

            if (strcmp((inputs->filtered)[i], "x") == 0)
                strcpy(filtered_cpy[i], double_to_str);
            i++;
        }

        start = 0;
        ans += calc(filtered_cpy, &start) * PRECISION;
    }

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
    if ((a >= 'A' && a <= 'z'))
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

    char element[MAX_FUNCTION_NAME], function[MAX_FUNCTION_NAME];
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
        double fact = 1;
        for (int i = 1; i <= num; i++)
        {
            fact = fact * i;
        }
        return fact;
    }

    if (strcmp(function, "PI") == 0)
    {
        return PI;
    }

    if(strcmp(function, "pi") == 0)
    {
        return 3.1415;
    }

    if(strcmp(function, "sin") == 0)
    {
        return sin(num);
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