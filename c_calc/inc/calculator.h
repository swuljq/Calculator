#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "cJSON.h"
#define STACK_MAX 256


int calculate(const char *operation, double a, double b, double *res, char *error_msg);
int GetResult(cJSON *json_expr, double *result, char *error_msg);

#endif 

