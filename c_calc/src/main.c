#include <stdio.h>
#include "calculator.h"
#include "mongoose.h"
#include "cJSON.h"

double result = 0.0;
char error_msg[128] = {0};

// HTTP请求处理函数
static void handle_request(struct mg_connection *c, int ev, void *ev_data) 
{
    if (ev == MG_EV_HTTP_MSG) 
    {
        

        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        printf("Received HTTP request: %.*s\n", (int)hm->uri.len, hm->uri.buf);
        if (mg_strcmp(hm->uri, mg_str("/calc")) == 0 &&
            _strnicmp(hm->method.buf, "POST", hm->method.len) == 0) 
        {

            char *body = (char *)malloc(hm->body.len + 1);
            memcpy(body, hm->body.buf, hm->body.len);
            body[hm->body.len] = '\0';
            printf("Received body: %s\n", body);
            // 解析 JSON数据
            cJSON *json = cJSON_Parse(body);
            free(body);
            if (!json) 
            {
                // JSON 解析失败，返回错误响应
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

            cJSON *expr = cJSON_GetObjectItem(json, "expression");
            if (!cJSON_IsArray(expr)) 
            {
                // "expression" 字段缺失或不是数组，返回错误响应
                cJSON *res = cJSON_CreateObject();
                cJSON_AddBoolToObject(res, "success", 0);
                cJSON_AddNullToObject(res, "result");
                cJSON_AddStringToObject(res, "error", "Missing or invalid 'expression' field, expected array");
                char *res_str = cJSON_PrintUnformatted(res);
                mg_http_reply(c, 400, "Content-Type: application/json\r\n", "%s", res_str);
                free(res_str);
                cJSON_Delete(res);
                cJSON_Delete(json);
                return;
            }
            // 计算后缀表达式结果
            if (GetResult(expr, &result, error_msg)) 
            {
                // 成功，构造成功响应
                cJSON *res = cJSON_CreateObject();
                cJSON_AddBoolToObject(res, "success", 1);
                cJSON_AddNumberToObject(res, "result", result);
                cJSON_AddNullToObject(res, "error");

                char *res_str = cJSON_PrintUnformatted(res);
                mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", res_str);
                free(res_str);
                cJSON_Delete(res);
            } 
            else 
            {
                // 失败，构造失败响应
                cJSON *res = cJSON_CreateObject();
                cJSON_AddBoolToObject(res, "success", 0);
                cJSON_AddNullToObject(res, "result");
                cJSON_AddStringToObject(res, "error", error_msg);

                char *res_str = cJSON_PrintUnformatted(res);
                mg_http_reply(c, 400, "Content-Type: application/json\r\n", "%s", res_str);
                free(res_str);
                cJSON_Delete(res);
            }
        } 
        else 
        {
            mg_http_reply(c, 404, "", "Not Found\n");
        }
    }
}

/**
 * @brief 主程序入口，初始化 HTTP 服务器
 * 
 * 程序启动后监听 8000 端口，等待客户端 HTTP 请求。
 * 使用 Mongoose 库管理事件循环。
 */
int main(void) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://0.0.0.0:8000", handle_request, NULL);
    printf("HTTP server started on port 8000\n");
    for (;;) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    return 0;
}