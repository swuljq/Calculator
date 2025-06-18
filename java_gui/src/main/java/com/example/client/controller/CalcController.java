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
     * 处理前端传来的数学表达式，调用中间层计算服务进行计算，并返回结果或错误信息
     *
     * @param expression 前端传来的数学表达式
     * @param model 用于将计算结果或错误信息传递给视图
     * @return 返回显示结果的视图名称
     */
    @PostMapping("/calculate")
    public String calculate(@RequestParam String expression, Model model) {
        // 创建 RestTemplate 用于向中间层服务发送请求
        RestTemplate restTemplate = new RestTemplate();

        // 设定中间层服务的 URL 地址
        String url = "http://172.20.10.4:8081/calculate"; // 此处填写中间层接口的URL

        // 设置请求头，告知服务器客户端发送的是 JSON 格式数据
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);

        // 构建请求体，包含数学表达式
        Map<String, String> payload = new HashMap<>();
        payload.put("expression", expression);
        HttpEntity<Map<String, String>> request = new HttpEntity<>(payload, headers);

        try {
            // 向中间层发送 POST 请求，获取计算结果
            ResponseEntity<Map> response = restTemplate.postForEntity(url, request, Map.class);

            // 检查响应结果是否包含计算结果
            if (response.getBody().containsKey("result")) {
                // 如果响应中包含计算结果，则传递给视图进行显示
                model.addAttribute("result", response.getBody().get("result"));
            } else {
                // 如果响应中包含错误信息，则传递错误信息给视图
                model.addAttribute("error", response.getBody().get("error"));
            }
        } catch (Exception e) {
            // 如果发生异常（如服务器无法连接等），则返回通用错误信息
            model.addAttribute("error", "连接服务器失败或表达式无效");
        }
        // 返回视图名称，用于展示计算结果或错误信息
        return "index";
    }

    /**
     * 处理根路径的请求，返回主页面视图
     *
     * @return 返回显示页面的视图名称
     */
    @GetMapping("/")
    public String index() {
        return "index"; // 返回主页面视图名称
    }
}
