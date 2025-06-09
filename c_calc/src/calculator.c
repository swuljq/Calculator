#include <stdio.h>
#include <string.h>
#include <math.h>
#include "calculator.h"

int calculate(const char *operation, double a, double b, double *res, char *error_msg) 
{
    if (strcmp(operation, "+") == 0 )
    {
        *res = a + b;
        return 1;
    }
    else if (strcmp(operation, "-") == 0 ) 
    {
        *res = a - b;
        return 1;
    }
    else if (strcmp(operation, "*") == 0 ) 
    {
        *res = a * b;
        return 1;
    }
    else if (strcmp(operation, "/") == 0 ) 
    {
        if (b == 0) 
        {
            strcpy(error_msg, "Division by zero");
            return 0;
        }
       *res = a / b;
       return 1;
    }
    else if (strcmp(operation, "sin") == 0) 
    {
        *res = sin(a * M_PI / 180.0);
        return 1;
    }
    else if (strcmp(operation, "cos") == 0) 
    {
        *res = cos(a * M_PI / 180.0);
        return 1;
    }
    else if (strcmp(operation, "tan") == 0)
    {
        *res = tan(a * M_PI / 180.0);
        return 1;
    }
    else if (strcmp(operation, "exp") == 0) 
    {
        *res = exp(a);
        return 1;
    }
    else if (strcmp(operation, "sqr") == 0) 
    {
        *res = a * a;
        return 1;
    }
    else if (strcmp(operation, "sqrt") == 0) 
    {
        if (a < 0) 
        {
            strcpy(error_msg, "The square root is negative");
            return 0;
        }
        *res = sqrt(a);
        return 1;
    }
    else if (strcmp(operation, "^") == 0) 
    {
        if (a < 0 && floor(b) != b)
        {
            strcpy(error_msg, "negative base with non-integer exponent");
            return 0;            
        }
        *res = pow(a, b);
        return 1;
    }
    else {
        strcpy(error_msg, "Unknown operation");
        return 0;
    }
}

// 定义栈结构体及相关操作，用于后缀表达式计算
typedef struct 
{
    double stack[STACK_MAX];
    int top;
} Stack;

// 初始化栈
static void stack_init(Stack *s) 
{
    s->top = -1;
}

// 压栈
static int stack_push(Stack *s, double val) 
{
    if (s->top >= STACK_MAX - 1)
    {
        return -1;
    }
    s->stack[++s->top] = val;
    return 0;
}

// 出栈
static int stack_pop(Stack *s, double *val) 
{
    if (s->top < 0) 
    {
        return -1;
    }
    *val = s->stack[s->top--];
    return 0;
}

/**
 * @brief 计算后缀表达式
 * 
 * @param json_expr 后缀表达式数组，元素是数字和操作符
 * @param result 计算结果输出指针
 * @param error_msg 存储错误信息
 * @return  0表示计算错误，1表示计算成功
 */
int GetResult(cJSON *json_expr, double *result, char *error_msg) 
{
    Stack stack;
    stack_init(&stack);

    int size = cJSON_GetArraySize(json_expr);

    for (int i = 0; i < size; i++) 
    {
        cJSON *item = cJSON_GetArrayItem(json_expr, i);
        if (cJSON_IsNumber(item)) 
        {
            // 数字元素，直接入栈
            stack_push(&stack, item->valuedouble);
        } 
        else if (cJSON_IsString(item)) 
        {
            // 运算符字符串，弹出对应操作数计算后结果入栈
            const char *op = item->valuestring;
            double a, b, res;
            
            // 单目运算符（例如 sin, cos, tan, sqrt, exp, sqr）
            if (strcmp(op, "sin") == 0 || strcmp(op, "cos") == 0 ||
                strcmp(op, "tan") == 0 || strcmp(op, "sqrt") == 0 ||
                strcmp(op, "exp") == 0 || strcmp(op, "sqr") == 0) 
            {
                stack_pop(&stack, &a);
                if (!calculate(op, a, 0, &res, error_msg))
                {
                    return 0;
                }
                stack_push(&stack, res); 
            }
            // 双目运算符（如 +, -, *, /, ^ 等） 
            else 
            {
                stack_pop(&stack, &b);
                stack_pop(&stack, &a);
                if (!calculate(op, a, b, &res, error_msg)) 
                {
                    return 0;
                }
                stack_push(&stack, res);
            }
        }
    }
    *result = stack.stack[stack.top];
    return 1;
}