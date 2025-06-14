package com.example.client.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import org.springframework.ui.Model;
import org.springframework.http.*;
import org.springframework.web.client.RestTemplate;

import java.util.*;

@Controller
public class CalcController {

    /**
     * 处理 /calculate 的 POST 请求，接收表达式参数，返回 JSON 格式的计算结果或错误信息
     */
    @PostMapping("/calculate")
    @ResponseBody  // 直接返回 JSON 数据，而不是页面视图
    public Map<String, String> calculate(@RequestParam String expression) {
        // 模拟开关（用于测试前端功能，无需依赖后端服务）
        boolean useMock = false;

        // 模拟模式：返回固定结果或错误
        if (useMock) {
            Map<String, String> mock = new HashMap<>();
            if (expression.contains("0")) {
                mock.put("error", "模拟错误：表达式不能包含0");
            } else {
                mock.put("result", "模拟结果：" + expression + " = 42");
            }
            return mock;
        }

        // 实际请求后端服务（如 Python 服务）进行表达式计算
        RestTemplate restTemplate = new RestTemplate();
        String url = "http://localhost:8081/calculate";  // 假设 Python 后端运行在 8081 端口

        // 设置请求头，指定为 JSON 格式
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);

        // 构造请求体（将表达式封装为 JSON）
        Map<String, String> payload = new HashMap<>();
        payload.put("expression", expression);
        HttpEntity<Map<String, String>> request = new HttpEntity<>(payload, headers);

        // 尝试发送 POST 请求，并获取响应
        try {
            ResponseEntity<Map> response = restTemplate.postForEntity(url, request, Map.class);
            return response.getBody();  // 成功返回后端返回的结果（可能是 result 或 error）
        } catch (Exception e) {
            // 捕获异常并返回错误信息（后端连接失败或语法错误等）
            Map<String, String> error = new HashMap<>();
            error.put("error", "连接服务器失败或表达式无效");
            return error;
        }
    }

    /**
     * 处理根路径 GET 请求，返回 index.html 页面（用于显示前端界面）
     */
    @GetMapping("/")
    public String index() {
        return "index";  // 返回 Thymeleaf 模板视图名称
    }
}
