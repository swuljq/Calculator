#include <stdio.h>
#include <math.h>
#include "calculator.h"

// 加法
double add(double a, double b) 
{
    return a + b;
}

// 减法
double subtract(double a, double b) 
{
    return a - b;
}

// 乘法
double multiply(double a, double b) 
{
    return a * b;
}

// 除法
double divide(double a, double b) 
{
    if (b == 0) {
        printf("Error: Division by zero!\n");
        return 0;
    }
    return a / b;
}

// 正弦函数
double sine(double degree) 
{
    return sin(degree * M_PI / 180.0);
}

// 余弦函数
double cosine(double degree) 
{
    return cos(degree * M_PI / 180.0);
}

// 正切函数
double tangent(double degree) 
{
    return tan(degree * M_PI / 180.0);
}

// 指数函数（e^x）
double exponential(double x) 
{
    return exp(x);
}

// 平方
double square(double x) 
{
    return x * x;
}

// 开方（需处理负数）
double square_root(double x) 
{
    if (x < 0) {
        printf("Error: Cannot take square root of a negative number!\n");
        return 0;
    }
    return sqrt(x);
}