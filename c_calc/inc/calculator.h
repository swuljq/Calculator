#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "cJSON.h"
#define STACK_MAX 256
#define MATH_ERR_DIV_ZERO -1
#define MATH_ERR_NEG_SQRT -2

double calculate(const char *operation, double a, double b);
int GetResult(cJSON *json_expr, double *result);

#endif 

