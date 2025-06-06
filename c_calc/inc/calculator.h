#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// 基本运算
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);

// 三角函数（角度制）
double sine(double degree);
double cosine(double degree);
double tangent(double degree);

// 指数与开方
double exponential(double x);    
double square(double x);          
double square_root(double x);     

#endif 

