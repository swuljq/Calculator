

# 接口文档：客户端

## 1. 接口基本信息

- **接口名称**：客户端控制接口
- **接口功能**：接收用户表达式输入，发送 HTTP POST 请求至中间层计算服务，处理响应并反馈前端结果。
- **作用**：接收用户输入的表达式，将其转化为 JSON 形式，通过 `RestTemplate` 向中间层服务 `http://localhost:8081/calculate` 发送 POST 请求。
- **处理逻辑**：
  - 若中间层返回结果字段 `result`，则展示计算结果；
  - 若返回错误字段 `error`，则展示错误消息；
  - 若请求异常（如服务器无法连接），则提示“连接服务器失败或表达式无效”。
- **接口路径**：`/calculate`  
- **请求方法**：`POST`  
- **请求类型**：`application/json`  

---

## 2. 请求参数

- **请求地址**：`POST http://<server-host>:<port>/calculate`  
- **请求头部**：`Content-Type: application/json`

### 请求体示例
```json
{
  "expression": "sin(30)+2^3"
}
```

| 字段名     | 类型    | 是否必填 | 说明                         |
|------------|---------|----------|------------------------------|
| expression | string  | 是       | 数学表达式（如：sin(30)+2^3） |

#### 页面表单提交示例（来自 HTML）

```html
<form method="post" action="/calculate">
  <input type="text" name="expression" required>
  <button type="submit">=</button>
</form>
```

---

## 3. 响应参数

### 成功响应示例
```json
{
  "result": 5.0
}
```

### 错误响应示例
```json
{
  "error": "Division by zero is undefined"
}
```

---

## 4. 请求与响应示例

### 请求
```http
POST /calculate HTTP/1.1
Host: localhost:8081
Content-Type: application/json

{
  "expression": "cos(60) + sqrt(9)"
}
```

### 响应
```json
{
  "result": 4.5
}
```

## 5. 界面展示逻辑（index.html）

- 若 `model` 中含有 `result`，展示计算结果；
- 若含有 `error`，展示错误信息；
- 默认加载空白输入框和“= 计算”按钮。

![1749128703758](https://github.com/user-attachments/assets/5bf30519-4948-4f4e-ac78-f30b75409847)
