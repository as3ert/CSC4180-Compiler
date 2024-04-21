'''
Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>

File Created: 17th April 2024
Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
Student ID: 120090244

Description: This code is for teaching purpose of course: CUHKSZ's CSC4180: Compiler Construction
             as Assignment 4: Oat v.1 Compiler Frontend to LLVM IR using llvmlite

Copyright (c)
Oat v.1 Language was designed by Prof.Steve Zdancewic when he was teaching CIS 341 at U Penn.
'''

import sys                          # for CLI argument parsing
import llvmlite.binding as llvm     # for llvmlite IR generation
import llvmlite.ir as ir            # for llvmlite IR generation
import pydot                        # for .dot file parsing
from enum import Enum               # for enum in python

DEBUG = False
scope_id = 1
temp_cnt = 1

class SymbolTable:
    """
    Symbol table is a stack of maps, and each map represents a scope

    The key of map is the lexeme (string, not unique) of an identifier, and the value is its type (DataType)

    The size of self.scopes and self.scope_ids should always be the same
    """
    def __init__(self):
        """
        Initialize the symbol table with no scope inside
        """
        self.id_counter = 0      # Maintain an increment counter for each newly pushed scope
        self.scopes = []         # map<str, DataType> from lexeme to data type in one scope
        self.scope_ids = []      # stores the ID for each scope

    def push_scope(self):
        """
        Push a new scope to symbol table

        Returns:
            - (int) the ID of the newly pushed scope
        """
        self.id_counter += 1
        self.scopes.append({})   # push a new table (mimics the behavior of stack)
        self.scope_ids.append(self.id_counter)
        return self.id_counter

    def pop_scope(self):
        """
        Pop a scope out of symbol table, usually called when the semantic analysis for one scope is finished
        """
        self.scopes.pop()    # pop out the last table (mimics the behavior of stack)
        self.scope_ids.pop()

    def unique_name(self, lexeme, scope_id):
        """
        Compute the unique name for an identifier in one certain scope

        Args:
            - lexeme(str)
            - scope_id(int)
        
        Returns:
            - str: the unique name of identifier used for IR codegen
        """
        return lexeme + "-" + str(scope_id)

    def insert(self, lexeme, type):
        """
        Insert a new symbol to the top scope of symbol table

        Args:
            - lexeme(str): lexeme of the symbol
            - type(DataType): type of the symbol
        
        Returns:
            - (str): the unique ID of the symbol
        """
        # check the size of scopes and scope_id
        if len(self.scopes) != len(self.scope_ids):
            raise ValueError("Mismatch size of symbol_table and id_table")
        scope_idx = len(self.scopes) - 1
        self.scopes[scope_idx][lexeme] = type
        return self.unique_name(lexeme, self.scope_ids[scope_idx])

    def lookup_local(self, lexeme):
        """
        Lookup a symbol in the top scope of symbol table only
        called when we want to declare a new local variable

        Args:
            - lexeme(str): lexeme of the symbol

        Returns:
            - (str, DataType): 2D tuple of unique_id and data type if the symbol is found
            - None if the symbol is not found
        """
        if len(self.scopes) != len(self.scope_ids):
            raise ValueError("Mismatch size of symbol_table and id_table")
        table_idx = len(self.scopes) - 1
        if lexeme in self.scopes[table_idx]:
            unique_name = self.unique_name(lexeme, self.scope_ids[table_idx])
            type = self.scopes[table_idx][lexeme]
            return unique_name, type
        else:
            return None

    def lookup_global(self, lexeme):
        """
        Lookup a symbol in all the scopes of symbol table (stack)
        called when we want to search a lexeme or declare a global variable

        Args:
            - lexeme(str): lexeme of the symbol

        Returns:
            - (str, DataType): 2D tuple of unique_id and data type if the symbol is found
            - None if the symbol is not found
        """
        if len(self.scopes) != len(self.scope_ids):
            raise ValueError("Mismatch size of symbol_table and id_table")
        for table_idx in range(len(self.scopes) - 1, -1, -1):
            if lexeme in self.scopes[table_idx]: 
                unique_name = self.unique_name(lexeme, self.scope_ids[table_idx])
                type = self.scopes[table_idx][lexeme]
                return unique_name, type
        return None
    
    def print_table(self):
        table_idx = len(self.scopes) - 1
        for lexeme in self.scopes[table_idx]:
            print(lexeme)

# Global Symbol Table for Semantic Analysis
symbol_table = SymbolTable()

# Global Context of LLVM IR Code Generation
module = ir.Module()                    # Global LLVM IR Module
builder = ir.IRBuilder()                # Global LLVM IR Builder
ir_map = {}                             # Global Map from unique names to its LLVM IR item

class TreeNode:
    def __init__(self, index, lexeme):
        self.index = index              # [int] ID of the TreeNode, used for visualization 
        self.lexeme = lexeme            # [str] lexeme of the node (may have naming conflicts, and needs unique name in IR codegen)
        self.id = ""                    # [str] unique name of the node (used in IR codegen), filled in semantic analysis
        self.nodetype = NodeType.NONE   # [NodeType] type of node, used to determine which actions to do with the current node in semantic analysis or IR codegen
        self.datatype = DataType.NONE   # [DataType] data type of node, filled in semantic analysis and used in IR codegen
        self.children = []              # Array of childern TreeNodes

    def add_child(self, child_node):
        self.children.append(child_node)

def print_tree(node, level = 0):
    print("  " * level + '|' + node.lexeme.replace("\n","\\n") + ", " + node.nodetype.name)
    for child in node.children:
        print_tree(child, level + 1)

def visualize_tree(root_node, output_path):
    """
    Visulize TreeNode in Graphviz

    Args:
        - root_node(TreeNode)
        - output_path(str): the output path for png file 
    """
    # construct pydot Node from TreeNode with label
    def pydot_node(tree_node):
        node = pydot.Node(tree_node.index)
        label = tree_node.nodetype.name
        if len(tree_node.id) > 0:
            label += "\n" + tree_node.id.replace("\\n","\/n")
        elif len(tree_node.lexeme) > 0:
            label += "\n" + tree_node.lexeme.replace("\\n","\/n")
        label += "\ntype: " + tree_node.datatype.name
        node.set("label", label)
        return node
    # Recursively visualize node
    def visualize(node, graph):
        # Add Root Node Only
        if node.index == 0:
            graph.add_node(pydot_node(node))
        # Add Children Nodes and Edges
        for child in node.children:
            graph.add_node(pydot_node(child))
            graph.add_edge(pydot.Edge(node.index, child.index))
            visualize(child, graph)
    # Output visualization png graph
    graph = pydot.Dot(graph_type="graph")
    visualize(root_node, graph)
    graph.write_png(output_path)

def construct_tree_from_dot(dot_filepath):
    """
    Read .dot file, which records the AST from parser

    Args:
        - dot_filepath(str): path of the .dot file

    Return:
        - TreeNode: the root node of the AST
    """
    # Extract the first graph from the list (assuming there is only one graph in the file)
    graph = pydot.graph_from_dot_file(dot_filepath)[0]
    # Initialize Python TreeNode structure
    nodes = []
    # code_type_map = { member.value: member for member in NodeType }
    # Add nodes
    for node in graph.get_nodes():
        if len(node.get_attributes()) == 0: continue
        index = int(node.get_name()[4:])
        # print(node.get_attributes(), node.get_attributes()['label'], node.get_attributes()['lexeme'])
        label = node.get_attributes()["label"][1:-1]    # exlcude enclosing quotes
        lexeme = node.get_attributes()['lexeme'][1:-1]  # exclude enclosing quotes
        tree_node = TreeNode(index, lexeme)
        tree_node.lexeme = lexeme
        tree_node.nodetype = { member.value: member for member in NodeType }[label] if any(label == member.value for member in NodeType) else NodeType.NONE
        if DEBUG: print("Index: ", index, ", lexeme: ", lexeme, ", nodetype: ", tree_node.nodetype)
        nodes.append(tree_node)
    # Add Edges
    for edge in graph.get_edges():
        src_id = int(edge.get_source()[4:])
        dst_id = int(edge.get_destination()[4:])
        nodes[src_id].add_child(nodes[dst_id])
    # root node should always be the first node
    return nodes[0]

class NodeType(Enum):
    """
    Map lexeme of AST node to code type in IR generation

    The string value here is to convert the token class by bison to Python NodeType
    """
    PROGRAM = "<program>"
    GLOBAL_DECL = "<global_decl>"     # global variable declaration
    FUNC_DECL = "<function_decl>"       # function declaration
    VAR_DECLS = "<var_decls>"
    VAR_DECL = "<var_decl>"        # variable declaration
    ARGS = "<args>"
    ARG = "<arg>"
    REF = "<ref>"
    GLOBAL_EXPS = "<global_exps>"
    STMTS = "<stmts>"
    EXPS = "<exps>"
    FUNC_CALL = "<func call>"       # function call
    ARRAY_INDEX = "<array index>"    # array element retrieval by index
    IF_STMT = "IF"         # if-else statement (nested case included)
    ELSE_STMT = "ELSE"
    FOR_LOOP = "FOR"        # for loop statement (nested case included)
    WHILE_LOOP = "WHILE"      # while loop statement (nested case included)
    RETURN = "RETURN"          # return statement
    NEW = "NEW"         # new variable with/without initialization
    ASSIGN = "ASSIGN"         # assignment (lhs = rhs)
    EXP = "<exp>"     # expression (including a lot of binary operators)
    TVOID = "void"
    TINT = "int"
    TSTRING = "string"
    TBOOL = "bool"
    NULL = "NULL"
    TRUE = "TRUE"
    FALSE = "FALSE"
    STAR = "STAR"
    PLUS = "PLUS"
    MINUS = "MINUS"
    LSHIFT = "LSHIFT"
    RLSHIFT = "RLSHIFT"
    RASHIFT = "RASHIFT"
    LESS = "LESS"
    LESSEQ = "LESSEQ"
    GREAT = "GREAT"
    GREATEQ = "GREATEQ"
    EQ = "EQ"
    NEQ = "NEQ"
    LAND = "LAND"
    LOR = "LOR"
    BAND = "BAND"
    BOR = "BOR"
    NOT = "NOT"
    TILDE = "TILDE"
    INTLITERAL = "INTLITERAL"
    STRINGLITERAL = "STRINGLITERAL"
    ID = "ID"
    NONE = "unknown"         # unsupported

class DataType(Enum):
    INT = 1             # INT refers to Int32 type (32-bit Integer)
    BOOL = 2            # BOOL refers to Int1 type (1-bit Integer, 1 for True and 0 for False)
    STRING = 3          # STRING refers to an array of Int8 (8-bit integer for a single character)
    INT_ARRAY = 4       # Array of integers, no need to support unless for bonus
    BOOL_ARRAY = 5      # Array of booleans, no need to support unless for bonus
    STRING_ARRAY = 6    # Array of strings, no need to support unless for bonus
    VOID = 7            # Void, you can choose whether to support it or not
    NONE = 8            # Unknown type, used as initialized value for each TreeNode

def ir_type(data_type, array_size = 1):
    map = {
        DataType.INT: ir.IntType(32),       # integer is in 32-bit
        DataType.BOOL: ir.IntType(32),      # bool is also in 32-bit, 0 for false and 1 for True
        DataType.STRING: ir.ArrayType(ir.IntType(8), array_size + 1),   # extra \0 (null terminator)
        # DataType.STRING: [ir.ArrayType(ir.IntType(8), array_size + 1), ir.IntType(8)],   # extra \0 (null terminator)
        DataType.INT_ARRAY: ir.ArrayType(ir.IntType(32), array_size),
        DataType.BOOL_ARRAY: ir.ArrayType(ir.IntType(32), array_size),
        DataType.STRING_ARRAY: ir.ArrayType(ir.ArrayType(ir.IntType(8), 32), array_size),   # string max size = 32 for string array 
    }
    type = map.get(data_type)
    if type:
        return type
    else:
        raise ValueError("Unsupported data type: ", data_type)

def declare_runtime_functions():
    """
    Declare built-in functions for Oat v.1 Language
    """
    # int32_t* array_of_string (char *str)
    func_type = ir.FunctionType(
        ir.PointerType(ir.IntType(32)),     # return type
        [ir.PointerType(ir.IntType(8))])    # args type
    # map function unique name in global scope to the function body
    # the global scope should have scope_id = 1 
    func = ir.Function(module, func_type, name="array_of_string")
    ir_map[SymbolTable.unique_name(symbol_table, "array_of_string", 1)] = func
    # char* string_of_array (int32_t *arr)
    func_type = ir.FunctionType(
        ir.PointerType(ir.IntType(8)),      # return type
        [ir.PointerType(ir.IntType(32))])   # args type
    func = ir.Function(module, func_type, name="string_of_array")
    ir_map[SymbolTable.unique_name(symbol_table, "string_of_array", 1)] = func
    # int32_t length_of_string (char *str)
    func_type = ir.FunctionType(
        ir.IntType(32),                     # return type
        [ir.PointerType(ir.IntType(8))])    # args type
    func = ir.Function(module, func_type, name="length_of_string")
    ir_map[SymbolTable.unique_name(symbol_table, "length_of_string", 1)] = func
    # char* string_of_int(int32_t i)
    func_type = ir.FunctionType(
        ir.PointerType(ir.IntType(8)),      # return type
        [ir.IntType(32)])                   # args type
    func = ir.Function(module, func_type, name="string_of_int")
    ir_map[SymbolTable.unique_name(symbol_table, "string_of_int", 1)] = func
    # char* string_cat(char* l, char* r)
    func_type = ir.FunctionType(
        ir.PointerType(ir.IntType(8)),      # return tyoe
        [ir.PointerType(ir.IntType(8)), ir.PointerType(ir.IntType(8))]) # args type
    func = ir.Function(module, func_type, name="string_cat")
    ir_map[SymbolTable.unique_name(symbol_table, "string_cat", 1)] = func
    # void print_string (char* str)
    func_type = ir.FunctionType(
        ir.VoidType(),                      # return type
        [ir.PointerType(ir.IntType(8))])    # args type
    func = ir.Function(module, func_type, name="print_string")
    ir_map[SymbolTable.unique_name(symbol_table, "print_string", 1)] = func
    # void print_int (int32_t i)
    func_type = ir.FunctionType(
        ir.VoidType(),                      # return type
        [ir.IntType(32)])                   # args type
    func = ir.Function(module, func_type, name="print_int")
    ir_map[SymbolTable.unique_name(symbol_table, "print_int", 1)] = func
    # void print_bool (int32_t i)
    func_type = ir.FunctionType(
        ir.VoidType(),                      # return type
        [ir.IntType(32)])                   # args type
    func = ir.Function(module, func_type, name="print_bool")
    ir_map[SymbolTable.unique_name(symbol_table, "print_bool", 1)] = func

def codegen(node):
    """
    Recursively do LLVM IR generation

    Call corresponding handler function for each NodeType

    Different NodeTypes may be mapped to the same handelr function

    Args:
        node(TreeNode)
    """
    codegen_func_map = {
        NodeType.GLOBAL_DECL: codegen_handler_global_decl,
        NodeType.FUNC_DECL: codegen_handler_function_decl,
        NodeType.VAR_DECL: codegen_handler_variable_decl,
        NodeType.FUNC_CALL: codegen_handler_function_call,
        NodeType.RETURN: codegen_handler_return,
        NodeType.ASSIGN: codegen_handler_assign,
    }
    codegen_func = codegen_func_map.get(node.nodetype)
    if codegen_func:
        codegen_func(node)
    else:
        codegen_handler_default(node)

# Some sample handler functions for IR codegen
def codegen_handler_default(node):
    for child in node.children:
        codegen(child)

def codegen_handler_global_decl(node):
    """
    Global variable declaration
    """
    global scope_id

    var = node.children[0]
    var_name = var.lexeme
    var_type = ir_type(var.datatype)
    var_value = int(node.children[1].lexeme)
    variable = ir.GlobalVariable(module, var_type, var_name)
    variable.linkage = "private"
    variable.global_constant = True
    variable.initializer = ir.Constant(var_type, var_value)

    ir_map[SymbolTable.unique_name(symbol_table, var_name, scope_id)] = variable

def codegen_handler_function_decl(node):
    """
    Function declaration
    """
    global scope_id, block

    type = node.children[0]
    info = node.children[1]
    args = node.children[2]
    stmt = node.children[3]

    name = info.lexeme
    func_type = type.datatype

    return_type = ir_type(func_type)
    arg_types = []

    for arg in args.children:
        arg_types.append(ir_type(arg.datatype))

    ftype = ir.FunctionType(
        return_type,
        arg_types
    )

    func = ir.Function(module, ftype, name)

    for i in range(len(args.children)):
        arg_name = args.children[i].lexeme
        func.args[i].name = arg_name
    
    ir_map[SymbolTable.unique_name(symbol_table, name, scope_id)] = func

    block = func.append_basic_block(name="entry")
    builder.position_at_end(block)

    scope_id += 1

    codegen(stmt)

    scope_id -= 1

def codegen_handler_variable_decl(node):
    """
    Local variable declaration
    """
    global scope_id

    id = node.children[0]
    rval = node.children[1]

    type = id.datatype
    name = id.lexeme
    lexeme = rval.lexeme

    typ = ir_type(type, len(lexeme))

    rval_content = lexeme
    if (type == DataType.INT or 
        type == DataType.BOOL):
        rval_content = int(lexeme)
    elif type == DataType.STRING:
        rval_content = rval_content + "\0"
        str_ptr = ir.Constant(typ, bytearray(rval_content, "utf-8"))

    var_ptr = builder.alloca(typ, name=name)

    if type == DataType.STRING:
        builder.store(str_ptr, var_ptr)
    else:
        var_cst = ir.Constant(typ, rval_content)
        builder.store(var_cst, var_ptr)

    ir_map[SymbolTable.unique_name(symbol_table, name, scope_id)] = var_ptr

def codegen_handler_function_call(node):
    """
    Function call
    """
    global scope_id, temp_cnt

    func_name = node.children[0]
    func_args = node.children[1]

    func = func_name.lexeme
    
    # Functions are always in scope 1
    fn = ir_map[SymbolTable.unique_name(symbol_table, func, 1)]

    args_ptr = []

    for arg in func_args.children:
        try:
            arg_local = ir_map[SymbolTable.unique_name(symbol_table, arg.lexeme, scope_id)]
        except:
            arg_local = ir_map[SymbolTable.unique_name(symbol_table, arg.lexeme, scope_id - 1)]
        
        args_ptr.append(arg_local)

    args = []

    for i in range(len(args_ptr)):
        if args_ptr[i].type.pointee == ir.IntType(32):
            args.append(builder.load(args_ptr[i], str(temp_cnt)))
            temp_cnt += 1
        elif args_ptr[i].type.pointee != ir.IntType(8):
            ptr = builder.gep(args_ptr[i], [ir.Constant(ir.IntType(32), 0), ir.Constant(ir.IntType(32), 0)])
            args.append(ptr)

    try:
        builder.call(fn, args)
    except:
        builder.call(fn, args_ptr)

def codegen_handler_return(node):
    """
    Return statement
    """
    global scope_id, temp_cnt

    val = node.children[0]

    val_local = ir.Constant(ir.IntType(32), 0)

    if val.nodetype != NodeType.INTLITERAL:
        try:
            val_local = ir_map[SymbolTable.unique_name(symbol_table, val.lexeme, scope_id)]
        except:
            val_local = ir_map[SymbolTable.unique_name(symbol_table, val.lexeme, scope_id - 1)]
    else:
        val_local = ir.Constant(ir.IntType(32), int(val.lexeme))

    val_loaded = val_local

    if (val.nodetype == NodeType.ID):
        val_loaded = builder.load(val_local, str(temp_cnt))
        temp_cnt += 1

    builder.ret(val_loaded)

def codegen_handler_plus(node):
    """
    Plus operation
    """
    global temp_cnt

    children = []

    for child in node.children:
        if child.nodetype == NodeType.ID:
            var_loaded = builder.load(ir_map[SymbolTable.unique_name(symbol_table, child.lexeme, scope_id)], str(temp_cnt))
            temp_cnt += 1
        else:
            var_loaded = ir.Constant(ir.IntType(32), int(child.lexeme))
        
        children.append(var_loaded)

    result = builder.add(children[0], children[1], str(temp_cnt))
    temp_cnt += 1

    return result

def codegen_handler_minus(node):
    """
    Minus operation
    """
    global temp_cnt

    children = []

    for child in node.children:
        if child.nodetype == NodeType.ID:
            var_loaded = builder.load(ir_map[SymbolTable.unique_name(symbol_table, child.lexeme, scope_id)], str(temp_cnt))
            temp_cnt += 1
        else:
            var_loaded = ir.Constant(ir.IntType(32), int(child.lexeme))
        
        children.append(var_loaded)

    result = builder.sub(children[0], children[1], str(temp_cnt))
    temp_cnt += 1

    return result

def codegen_handler_assign(node):
    """
    Assignment operation
    """
    global scope_id, temp_cnt

    lval = node.children[0]
    rval = node.children[1]

    lval_local = ir_map[SymbolTable.unique_name(symbol_table, lval.lexeme, scope_id)]

    rval_local = ir.Constant(ir.IntType(32), 0)

    if rval.nodetype == NodeType.INTLITERAL:
        rval_local = ir.Constant(ir.IntType(32), int(rval.lexeme))
    elif rval.nodetype == NodeType.ID:
        try:
            rval_local = ir_map[SymbolTable.unique_name(symbol_table, rval.lexeme, scope_id)]
        except:
            rval_local = ir_map[SymbolTable.unique_name(symbol_table, rval.lexeme, scope_id - 1)]
        rval_local = builder.load(rval_local, str(temp_cnt))
        temp_cnt += 1
    elif rval.nodetype == NodeType.PLUS:
        rval_local = codegen_handler_plus(rval)
    elif rval.nodetype == NodeType.MINUS:
        rval_local = codegen_handler_minus(rval)

    builder.store(rval_local, lval_local)

def semantic_analysis(node):
    """
    Perform semantic analysis on the root_node of AST

    Args:
        node(TreeNode)

    Returns:
        (DataType): datatype of the node
    """
    handler_map = {
        NodeType.PROGRAM: semantic_handler_program,
        NodeType.GLOBAL_DECL: semantic_handler_global_decl,
        NodeType.VAR_DECL: semantic_handler_variable_decl,
        NodeType.FUNC_DECL: semantic_handler_function_decl,
        NodeType.RETURN: semantic_handler_return,

        NodeType.ID: semantic_handler_id,
        NodeType.TINT: semantic_handler_int,
        NodeType.TBOOL: semantic_handler_bool,
        NodeType.TSTRING: semantic_handler_string,
        NodeType.INTLITERAL: semantic_handler_int,
        NodeType.STRINGLITERAL: semantic_handler_string,
        NodeType.TRUE: semantic_handler_bool,
        NodeType.FALSE: semantic_handler_bool,
        NodeType.ARGS:semantic_handler_void,

        NodeType.STMTS: semantic_handler_statements,
    }
    handler = handler_map.get(node.nodetype)
    if handler:
        handler(node)
    else:
        default_handler(node)
    return node.datatype

def semantic_handler_program(node):
    symbol_table.push_scope()
    # insert built-in function names in global scope symbol table
    symbol_table.insert("array_of_string", DataType.INT_ARRAY)
    symbol_table.insert("string_of_array", DataType.STRING)
    symbol_table.insert("length_of_string", DataType.INT)
    symbol_table.insert("string_of_int", DataType.STRING)
    symbol_table.insert("string_cat", DataType.STRING)
    symbol_table.insert("print_string", DataType.VOID)
    symbol_table.insert("print_int", DataType.VOID)
    symbol_table.insert("print_bool", DataType.BOOL)
    # recursively do semantic analysis in left-to-right order for all children nodes
    for child in node.children:
        semantic_analysis(child)
    symbol_table.pop_scope()

def semantic_handler_global_decl(node):
    # Do semantic analysis on the right hand side of the asssignment operation
    semantic_analysis(node.children[1])

    # Insert lexeme and data type for the global declaration
    symbol_table.insert(node.children[0].lexeme, node.children[1].datatype)

    # Do semantic analysis on the left hand side of the assignment operation
    semantic_analysis(node.children[0]) 

def semantic_handler_variable_decl(node):
    # Do semantic analysis on the right hand side of the asssignment operation
    semantic_analysis(node.children[1])

    # Insert lexeme and data type for the local declaration
    symbol_table.insert(node.children[0].lexeme, node.children[1].datatype)

    # Do semantic analysis on the left hand side of the assignment operation
    semantic_analysis(node.children[0]) 

def semantic_handler_variable_decl(node):
    # Do semantic analysis on the right hand side of the asssignment operation
    semantic_analysis(node.children[1])

    local_var = node.children[0]
    lexeme = local_var.lexeme
    data_type = node.children[1].datatype

    symbol_table.insert(lexeme, data_type)
    if symbol_table.lookup_local(lexeme) is None:
        raise ValueError("Variable not defined: ", lexeme)
    else:
        local_var.id, local_var.datatype = symbol_table.lookup_local(lexeme)
    
def semantic_handler_function_decl(node):
    # Do semantic analysis on the type definition
    semantic_analysis(node.children[0])

    # Insert lexeme and data type for the function declaration
    symbol_table.insert(node.children[1].lexeme, node.children[0].datatype)
    
    # Create new scope for local variables
    symbol_table.push_scope()

    for child in node.children:
        semantic_analysis(child)

    symbol_table.pop_scope()

def semantic_handler_return(node):
    for child in node.children:
        semantic_analysis(child)
        node.datatype = child.datatype

def semantic_handler_statements(node):
    for child in node.children:
        semantic_analysis(child)
        node.datatype = child.datatype

# Some Sample handler functions
def semantic_handler_id(node):
    if symbol_table.lookup_global(node.lexeme) is None:
        raise ValueError("Variable not defined: ", node.lexeme)
    else:
        node.id, node.datatype = symbol_table.lookup_global(node.lexeme)

def semantic_handler_int(node):
    node.datatype = DataType.INT

def semantic_handler_bool(node):
    node.datatype = DataType.BOOL

def semantic_handler_string(node):
    node.datatype = DataType.STRING

def semantic_handler_void(node):
    node.datatype = DataType.VOID

def default_handler(node):
    for child in node.children:
        semantic_analysis(child)

if len(sys.argv) == 3:
    # visualize AST before semantic analysis
    dot_path = sys.argv[1]
    ast_png_before_semantic_analysis = sys.argv[2]
    root_node = construct_tree_from_dot(dot_path)
    if DEBUG: print_tree(root_node)
    visualize_tree(root_node, ast_png_before_semantic_analysis)
elif len(sys.argv) == 4:
    # visualize AST after semantic analysis
    dot_path = sys.argv[1]
    ast_png_after_semantics_analysis = sys.argv[2]
    llvm_ir = sys.argv[3]
    root_node = construct_tree_from_dot(dot_path)
    semantic_analysis(root_node)
    visualize_tree(root_node, ast_png_after_semantics_analysis)
    # # Uncomment the following when you are trying the do IR generation
    # # init llvm
    # llvm.initialize()
    # llvm.initialize_native_target()
    # llvm.initialize_native_asmprinter()
    # declare_runtime_functions()
    # codegen(root_node)
    # # print LLVM IR
    # print(module)
    # # Save module into LLVM IR
    # with open(llvm_ir, "w") as file:
    #     file.write(str(module))
else:
    raise SyntaxError("Usage: python3 a4.py <.dot> <.png before>\nUsage: python3 ./a4.py <.dot> <.png after> <.ll>")
