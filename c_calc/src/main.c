#include <stdio.h>
#include "calculator.h"
#include "mongoose.h"
#include "cJSON.h"

double calculate(const char *operation, double a, double b) 
{
    if (strcmp(operation, "add") == 0)
    {
        return add(a , b);
    }
    else if (strcmp(operation, "sub") == 0)
    {
        return subtract(a, b);
    }
    else if (strcmp(operation, "mul") == 0)
    {
        return multiply(a, b);
    }
    else if (strcmp(operation, "div") == 0 && b != 0)
    {
        return divide(a, b);
    }
    else if (strcmp(operation, "exp") == 0)
    {
        return exponential(a);
    }
    else if (strcmp(operation, "sqr") == 0)
    {
        return square(a);
    }
    else if (strcmp(operation, "sqrt") == 0)
    {
        return square_root(a);
    }
    else if (strcmp(operation, "sin") == 0)
    {
        return sine(a);
    }
    else if (strcmp(operation, "cos") == 0)
    {
        return cosine(a);
    }
    else if (strcmp(operation, "tan") == 0)
    {
        return tangent(a);
    }
    else
    {
        return 0;
    }
}
// HTTP请求处理函数
static void handle_request(struct mg_connection *c, int ev, void *ev_data)
{
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        if (mg_strcmp(hm->uri, mg_str("/calc")) == 0 &&
                _strnicmp(hm->method.buf, "POST", hm->method.len) == 0) {
            
            char *body = (char *)malloc(hm->body.len + 1);
            memcpy(body, hm->body.buf, hm->body.len);
            body[hm->body.len] = '\0';

            // 解析JSON
            cJSON *json = cJSON_Parse(body);
            free(body);
            if (!json) 
            {
                cJSON *res = cJSON_CreateObject();
                cJSON_AddBoolToObject(res, "success", 0);
                cJSON_AddNullToObject(res, "result");
                cJSON_AddStringToObject(res, "error", "Invalid JSON format");
                char *res_str = cJSON_PrintUnformatted(res);
                mg_http_reply(c, 400, "Content-Type: application/json\r\n", "%s", res_str);
                free(res_str);
                cJSON_Delete(res);
                return;
            }

            cJSON *op = cJSON_GetObjectItem(json, "operation");
            cJSON *a = cJSON_GetObjectItem(json, "a");
            cJSON *b = cJSON_GetObjectItem(json, "b");

            if (!cJSON_IsString(op) || !cJSON_IsNumber(a) || !cJSON_IsNumber(b)) 
            {
                cJSON *res = cJSON_CreateObject();
                cJSON_AddBoolToObject(res, "success", 0);
                cJSON_AddNullToObject(res, "result");
                cJSON_AddStringToObject(res, "error", "Missing or invalid parameters");
                char *res_str = cJSON_PrintUnformatted(res);
                mg_http_reply(c, 400, "Content-Type: application/json\r\n", "%s", res_str);
                free(res_str);
                cJSON_Delete(res);
                cJSON_Delete(json);
                return;
            }

            double result = calculate(op->valuestring, a->valuedouble, b->valuedouble);
            cJSON_Delete(json);

            // 返回结果JSON
            cJSON *res_json = cJSON_CreateObject();
             cJSON_AddBoolToObject(res_json, "success", 1);
            cJSON_AddNumberToObject(res_json, "result", result);
            cJSON_AddNullToObject(res_json, "error");
            char *res_str = cJSON_PrintUnformatted(res_json);
            cJSON_Delete(res_json);

            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", res_str);
            free(res_str);
        } 
        else 
        {
            mg_http_reply(c, 404, "", "Not Found\n");
        }
    }
}

int main(void) 
{
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    // 监听端口 8000
    mg_http_listen(&mgr, "http://0.0.0.0:8000", handle_request, NULL);

    printf("HTTP server started on port 8000\n");

    for (;;) mg_mgr_poll(&mgr, 1000);

    mg_mgr_free(&mgr);
    return 0;
}
