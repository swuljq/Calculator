from flask import Flask, request, jsonify
import re
import math

app = Flask(__name__)
SERVER_HOST = 'localhost'
SERVER_PORT = 8090


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
                  'sin': 5, 'cos': 5, 'tan': 5,  # 三角函数最高优先级
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
            elements[i-1] = -elements[i-1]

    rpn = list(filter(lambda x: x != '~', elements))
    return rpn




@app.route('/calculate', methods=['POST'])
def calculate():
    data = request.get_json()
    if not data or 'expression' not in data:
        return jsonify({"error": "Invalid JSON format"}), 400

    original_expr = data['expression']
    print(original_expr)


       # if not is_valid_expression(expr):
        #    print('abc')
         #   return jsonify({"error": "Invalid expression (unmatched parentheses)"}), 400

    rpn = infix_to_rpn(original_expr)

    print({"error": rpn})
    return jsonify({"error": rpn}), 400




if __name__ == '__main__':
    print("中间层服务已启动，监听端口: 8081")
    app.run(host='localhost', port=8081, debug=True)
