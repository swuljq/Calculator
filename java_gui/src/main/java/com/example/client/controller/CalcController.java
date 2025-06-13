package com.example.client.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import org.springframework.ui.Model;
import org.springframework.http.*;
import org.springframework.web.client.RestTemplate;

import java.util.*;



@Controller
public class CalcController {

    @PostMapping("/calculate")
    @ResponseBody
    public Map<String, String> calculate(@RequestParam String expression) {
        // 模拟开关
        boolean useMock = true;

        if (useMock) {
            Map<String, String> mock = new HashMap<>();
            if (expression.contains("0")) {
                mock.put("error", "模拟错误：表达式不能包含0");
            } else {
                mock.put("result", "模拟结果：" + expression + " = 42");
            }
            return mock;
        }

        // 如果 useMock 为 false，执行真实后端调用逻辑
        RestTemplate restTemplate = new RestTemplate();
        String url = "http://localhost:8081/calculate";

        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);

        Map<String, String> payload = new HashMap<>();
        payload.put("expression", expression);
        HttpEntity<Map<String, String>> request = new HttpEntity<>(payload, headers);

        try {
            ResponseEntity<Map> response = restTemplate.postForEntity(url, request, Map.class);
            return response.getBody();
        } catch (Exception e) {
            Map<String, String> error = new HashMap<>();
            error.put("error", "连接服务器失败或表达式无效");
            return error;
        }
    }

    @GetMapping("/")
    public String index() {
        return "index";
    }
}
