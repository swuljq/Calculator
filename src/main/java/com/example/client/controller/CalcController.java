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
    public String calculate(@RequestParam String expression, Model model) {
        RestTemplate restTemplate = new RestTemplate();
        String url = "http://localhost:8081/calculate";

        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);

        Map<String, String> payload = new HashMap<>();
        payload.put("expression", expression);
        HttpEntity<Map<String, String>> request = new HttpEntity<>(payload, headers);

        try {
            ResponseEntity<Map> response = restTemplate.postForEntity(url, request, Map.class);
            if (response.getBody().containsKey("result")) {
                model.addAttribute("result", response.getBody().get("result"));
            } else {
                model.addAttribute("error", response.getBody().get("error"));
            }
        } catch (Exception e) {
            model.addAttribute("error", "连接服务器失败或表达式无效");
        }
        return "index";
    }

    @GetMapping("/")
    public String index() {
        return "index";
    }
}
