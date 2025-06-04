#include <stdio.h>
#include "calculator.h"

int main() {
    double a = 16.4, b = 4.2;
    printf("Add: %f + %f = %f\n", a, b, add(a, b));
    printf("Subtract: %f - %f = %f\n", a, b, subtract(a, b));
    printf("Multiply: %f * %f = %f\n", a, b, multiply(a, b));
    printf("Divide: %f / %f = %f\n", a, b, divide(a, b));

    printf("Sine(%f): %f\n", 30.0, sine(30.0));
    printf("Cosine(%f): %f\n", 60.0, cosine(60.0));
    printf("Tangent(%f): %f\n", 45.0, tangent(45.0));

    printf("Exponential: e^%f = %f\n", 3.0, exponential(3.0));
    printf("Square:  %f^2 = %f\n", 5.0, square(5.0));
    printf("Square root(%f):%f\n", 36.0, square_root(36.0));

    // 测试除零情况
    printf("Divide by zero (%.2f / 0): %f\n", a, divide(a, 0));

    // 测试负数开方
    double neg = -4.0;
    printf("Square root of negative number (%f): %f\n", neg, square_root(neg));
    return 0;
}
