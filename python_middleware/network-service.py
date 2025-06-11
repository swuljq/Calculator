from flask import Flask, request, jsonify
import re
import socket
import json
import requests

app = Flask(__name__)
SERVER_HOST = '192.168.162.179' 
SERVER_PORT = 8000


def convert_if_number(s):
    try:
        return int(s)  # 先尝试转整数
    except ValueError:
        try:
            return float(s)  # 失败则尝试转浮点数
        except ValueError:
            return s  # 仍然失败，则返回原字符串


def is_valid_expression(expr):
    """检查表达式是否合法（括号匹配）"""
    stack = []
    for char in expr:
        if char == '(':
            stack.append(char)
        elif char == ')':
            if not stack:
                return False
            stack.pop()
    return len(stack) == 0


def infix_to_rpn(expression):
    """将中缀表达式转换为逆波兰表示法（RPN）"""
    # 定义运算符优先级
    precedence = {'u-': 6,
                  'sin': 5, 'cos': 5, 'tan': 5,  
                  '^': 4,
                  '*': 3, '/': 3,
                  '+': 2, '-': 2,
                  '(': 1
                  }

    expression = expression.replace(' ', '')  # 移除所有空格

    # 使用正则表达式匹配数字、运算符、函数和括号
    tokens = re.findall(r"(\d+\.?\d*|[a-zA-Z]+|[-+*/^()])", expression)

    output = []
    operator_stack = []

    for i, token in enumerate(tokens):
        if token.replace('.', '', 1).isdigit():  # 数字
            output.append(token)
        elif token in ['sin', 'cos', 'tan']:  # 函数
            operator_stack.append(token)
        elif token == '(':  # 左括号
            operator_stack.append(token)
        elif token == ')':  # 右括号
            while operator_stack and operator_stack[-1] != '(':
                output.append(operator_stack.pop())
            operator_stack.pop()  # 弹出左括号

            # 如果栈顶是函数，弹出并输出
            if operator_stack and operator_stack[-1] in ['sin', 'cos', 'tan']:
                output.append(operator_stack.pop())

        else:  # 如果是操作符
            # 处理负号（负数）,表达式开头，前一个操作符的后面
            if token == '-' and (i == 0 or tokens[i - 1] in "+-*/("):
                token = 'u-'  # 标记为负号
            while operator_stack and precedence.get(token, 0) <= precedence.get(operator_stack[-1], 0):
                output.append(operator_stack.pop())

            operator_stack.append(token)

    # 弹出栈中剩余运算符
    while operator_stack:
        output.append(operator_stack.pop())

    # 将一元负号转换为更标准的表示
    rpn = ' '.join(output)
    rpn = rpn.replace('u-', '~')
    elements = rpn.split()

    for i in range(len(elements)):
        elements[i] = convert_if_number(elements[i])
        if elements[i] == '~':
            elements[i - 1] = -elements[i - 1]

    rpn = list(filter(lambda x: x != '~', elements))
    return rpn

def is_syntax_valid(expr):
    # 判断表示式语法是否正确
    expr = expr.replace(' ', '')  # 去除空格
    if not expr:
        return False

    # 合法字符（数字、字母、运算符、括号）
    valid_chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-*/^()."
    for ch in expr:
        if ch not in valid_chars:
            return False

    operators = set("+-*/^")
    prev_char = ''
    for i, ch in enumerate(expr):
        # 表达式不能以运算符（除了负号）开头
        if i == 0 and ch in "*/^":
            return False
        # 连续两个运算符不合法，负号除外（负号可能是一元）
        if ch in operators:
            if prev_char in operators:
                # 允许 '-' 作为负号，只要不是连续两个非 '-' 运算符
                if not (ch == '-' and prev_char != '-'):
                    return False
        # 运算符后面不能直接跟右括号
        if prev_char in operators and ch == ')':
            return False
        # 左括号后面不能直接跟运算符（除了负号和左括号）
        if prev_char == '(' and ch in "*/^+)":
            return False
        # 右括号后面不能直接跟数字或字母
        if prev_char == ')' and (ch.isalnum() or ch == '('):
            return False
        prev_char = ch

    # 表达式不能以运算符结尾
    if expr[-1] in operators:
        return False

    return True


def send_to_server(data):
    # 向计算端发送数据以及接收数据
    try:
        url = f"http://{SERVER_HOST}:{SERVER_PORT}/calc"  # 根路径 POST
        headers = {"Content-Type": "application/json"}
        print(url)
        response = requests.post(url, json=data, headers=headers, timeout=5)
        print(response.status_code)
        return response.json()
    except Exception as e:
        return {'success': False, 'error': '服务端通信错误', 'result': None}

@app.route('/calculate', methods=['POST'])
def calculate():
    try:
        data = request.get_json()

        if not data or 'expression' not in data:  # 判断表达式是否为空
            return jsonify({"error": "无效 JSON 数据"})


        original_expr = data['expression']

        if not is_valid_expression(original_expr):  # 判断括号是否匹配
            return jsonify({"error": "错误表达式 (括号不匹配)"})

        if not is_syntax_valid(original_expr):  # 判断表达式语法是否正确
            return jsonify({"error": "表达式语法错误"})


        rpn = infix_to_rpn(original_expr)

        result = send_to_server({"expression": rpn})

        if result.get('success'):
            # 计算成功，获取结果
            value = result.get('result')
            return jsonify({"result": value})
        else:
            # 计算失败，返回错误信息
            error_msg = result.get('error', '未知错误')
            return jsonify({"error": error_msg})

    except Exception as e:
        return jsonify({'error': f'处理请求时出错'})


if __name__ == '__main__':
    print("中间层服务已启动，监听端口: 8081")
    app.run(host='localhost', port=8081, debug=True)
