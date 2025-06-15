🧩 接口一：表达式计算接口（/calculate）
1. 接口描述
   用于接收用户输入的数学表达式，将其发送至后端计算服务（如 Python 服务），并返回计算结果或错误信息。

2. 接口地址
   POST /calculate

3. 请求参数
   参数名	类型	是否必须	描述
   expression	string	是	要计算的数学表达式（如 sin(30) + 2^3）

请求格式类型：

Content-Type: application/x-www-form-urlencoded

4. 请求示例（前端 JS 发起 AJAX 请求）
   javascript
   复制
   编辑
   fetch('/calculate', {
   method: 'POST',
   headers: {
   'Content-Type': 'application/x-www-form-urlencoded'
   },
   body: new URLSearchParams({expression: "2 + 2"})
   })
5. 响应参数
   成功时（HTTP 200）返回：

json
复制
编辑
{
"result": "4"
}
失败时返回：

json
复制
编辑
{
"error": "表达式无效或计算错误"
}
或（服务连接失败）：

json
复制
编辑
{
"error": "连接服务器失败或表达式无效"
}
6. 响应说明
   字段名	类型	描述
   result	string	表达式计算成功的结果（优先）
   error	string	错误信息

返回字段中 result 与 error 至少存在一个，前端需要判断处理。

7. 异常情况
   表达式非法或无法计算（如 2 + * 3）；

后端 Python 计算服务未启动或连接失败；

用户传入空字符串或格式不符。

8. 后端逻辑说明
   请求发送至 http://localhost:8081/calculate（Python 服务），以 JSON 格式传递表达式。

如果开启模拟（useMock = true），会直接返回伪造结果或错误，用于前端测试。

捕获计算服务连接异常时，将统一返回 "连接服务器失败或表达式无效" 错误信息。

🧩 接口二：首页展示接口（/）
1. 接口描述
   访问根路径 / 时，返回前端界面页面（计算器页面）。

2. 接口地址
   GET /

3. 请求参数
   无

4. 响应
   返回前端页面 index.html，通过 Thymeleaf 模板渲染，内含表单、按钮、JS 逻辑等。

5. 示例截图（页面效果）
   略，可结合你页面效果图

6. 技术说明
   页面中表单不会直接刷新页面，而是通过 JavaScript 拦截表单提交，使用 fetch() 向 /calculate 发送请求；

页面响应结果将动态展示在下方 <p> 标签中（绿色为结果，红色为错误）；
![](E:\git\Calculator\java_gui\img.png)
页面内置数学函数按钮（sin, cos, tan, ^ 等）供快速输入使用。

