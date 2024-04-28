class Node:
    def __init__(self, label, lexeme=""):
        self.label = label
        self.lexeme = lexeme
        self.children = []

    def add_child(self, node):
        self.children.append(node)

def create_ast(tokens):
    stack = []
    current_node = Node("<program>")

    def push_node(label):
        nonlocal current_node
        new_node = Node(label)
        current_node.add_child(new_node)
        stack.append(current_node)
        current_node = new_node

    def pop_node():
        nonlocal current_node
        current_node = stack.pop()

    token_iter = iter(tokens)
    for token in token_iter:
        typ, val = token
        if typ == 'TINT':
            push_node('<function_decl>')
            node = Node('int', val)
            current_node.add_child(node)
        elif typ == 'ID':
            node = Node('ID', val)
            current_node.add_child(node)
            if val == 'main':
                push_node('<args>')
                pop_node()  # Args is empty, immediately pop
                push_node('<stmts>')
        elif typ == 'STRINGLITERAL':
            node = Node('STRINGLITERAL', val)
            current_node.add_child(node)
        elif typ == 'INTLITERAL':
            node = Node('INTLITERAL', val)
            current_node.add_child(node)
        elif typ == 'LPAREN' or typ == 'RPAREN':
            continue
        elif typ == 'LBRACE':
            continue  # Already handling with function declaration
        elif typ == 'RBRACE':
            pop_node()  # Close <stmts>
            pop_node()  # Close <function_decl>
        elif typ == 'VAR':
            push_node('<var_decl>')
        elif typ == 'SEMICOLON':
            pop_node()
        elif typ == 'RETURN':
            push_node('RETURN')

    return current_node

def generate_dot(node):
    dot_output = ["digraph AST {"]
    id_counter = 0

    def add_nodes(node):
        nonlocal id_counter
        current_id = id_counter
        label = f'{node.label}'
        if node.lexeme:
            label += f', lexeme="{node.lexeme}"'
        dot_output.append(f'node{id_counter} [label="{label}"];')
        parent_id = id_counter
        id_counter += 1
        for child in node.children:
            child_id = add_nodes(child)
            dot_output.append(f'node{parent_id} -> node{child_id};')
        return parent_id

    add_nodes(node)
    dot_output.append("}")
    return "\n".join(dot_output)

tokens = [
    ('TINT', 'int'), ('ID', 'main'), ('LPAREN', '('), ('RPAREN', ')'),
    ('LBRACE', '{'), ('VAR', 'var'), ('ID', 'str'), ('ASSIGN', '='),
    ('STRINGLITERAL', '"hello world!"'), ('SEMICOLON', ';'),
    ('ID', 'print_string'), ('LPAREN', '('), ('ID', 'str'), ('RPAREN', ')'),
    ('SEMICOLON', ';'), ('RETURN', 'return'), ('INTLITERAL', '0'), ('SEMICOLON', ';'),
    ('RBRACE', '}'), ('EOF', '')
]

ast_root = create_ast(tokens)
dot_output = generate_dot(ast_root)
print(dot_output)

# To write to a file, uncomment the following lines:
# with open('ast_output.dot', 'w') as file:
#     file.write(dot_output)
