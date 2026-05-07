/*
 * ============================================================
 *   MINI-C COMPILER — AST Implementation
 *   File: ast.c
 *   Purpose: Implements AST node constructors and the
 *            tree printer for Phase 2 output display.
 * ============================================================
 */

#include "ast.h"

/* ──────────────────────────────────────────────
 * Helper: Allocate and zero-initialize a node
 * ────────────────────────────────────────────── */
static ASTNode *alloc_node(NodeType type, int line) {
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for AST node\n");
        exit(1);
    }
    node->type = type;
    node->line_number = line;
    return node;
}

/* Helper: duplicate a string safely */
static char *safe_strdup(const char *s) {
    if (!s) return NULL;
    char *dup = strdup(s);
    if (!dup) {
        fprintf(stderr, "Error: Memory allocation failed for string\n");
        exit(1);
    }
    return dup;
}

/* ──────────────────────────────────────────────
 * Constructor Functions — One per node type
 * ────────────────────────────────────────────── */

ASTNode *create_program_node(int line) {
    ASTNode *n = alloc_node(NODE_PROGRAM, line);
    n->data.program.declarations = (ASTNode **)calloc(MAX_CHILDREN, sizeof(ASTNode *));
    n->data.program.decl_count = 0;
    return n;
}

ASTNode *create_var_decl_node(const char *type, const char *name, ASTNode *init, int line) {
    ASTNode *n = alloc_node(NODE_VAR_DECL, line);
    n->data.var_decl.var_type = safe_strdup(type);
    n->data.var_decl.name = safe_strdup(name);
    n->data.var_decl.init_expr = init;
    return n;
}

ASTNode *create_fun_decl_node(const char *ret_type, const char *name,
                               ASTNode **params, int param_count,
                               ASTNode *body, int line) {
    ASTNode *n = alloc_node(NODE_FUN_DECL, line);
    n->data.fun_decl.return_type = safe_strdup(ret_type);
    n->data.fun_decl.name = safe_strdup(name);
    n->data.fun_decl.params = params;
    n->data.fun_decl.param_count = param_count;
    n->data.fun_decl.body = body;
    return n;
}

ASTNode *create_param_node(const char *type, const char *name, int line) {
    ASTNode *n = alloc_node(NODE_PARAM, line);
    n->data.param.param_type = safe_strdup(type);
    n->data.param.name = safe_strdup(name);
    return n;
}

ASTNode *create_compound_node(int line) {
    ASTNode *n = alloc_node(NODE_COMPOUND_STMT, line);
    n->data.compound.items = (ASTNode **)calloc(MAX_CHILDREN, sizeof(ASTNode *));
    n->data.compound.item_count = 0;
    return n;
}

ASTNode *create_if_node(ASTNode *cond, ASTNode *then_b, ASTNode *else_b, int line) {
    ASTNode *n = alloc_node(NODE_IF_STMT, line);
    n->data.if_stmt.condition = cond;
    n->data.if_stmt.then_branch = then_b;
    n->data.if_stmt.else_branch = else_b;
    return n;
}

ASTNode *create_while_node(ASTNode *cond, ASTNode *body, int line) {
    ASTNode *n = alloc_node(NODE_WHILE_STMT, line);
    n->data.while_stmt.condition = cond;
    n->data.while_stmt.body = body;
    return n;
}

ASTNode *create_for_node(ASTNode *init, ASTNode *cond, ASTNode *update, ASTNode *body, int line) {
    ASTNode *n = alloc_node(NODE_FOR_STMT, line);
    n->data.for_stmt.init = init;
    n->data.for_stmt.condition = cond;
    n->data.for_stmt.update = update;
    n->data.for_stmt.body = body;
    return n;
}

ASTNode *create_return_node(ASTNode *expr, int line) {
    ASTNode *n = alloc_node(NODE_RETURN_STMT, line);
    n->data.return_stmt.expr = expr;
    return n;
}

ASTNode *create_expr_stmt_node(ASTNode *expr, int line) {
    ASTNode *n = alloc_node(NODE_EXPR_STMT, line);
    n->data.expr_stmt.expr = expr;
    return n;
}

ASTNode *create_printf_node(const char *fmt, ASTNode **args, int arg_count, int line) {
    ASTNode *n = alloc_node(NODE_PRINTF_STMT, line);
    n->data.printf_stmt.format_str = safe_strdup(fmt);
    n->data.printf_stmt.args = args;
    n->data.printf_stmt.arg_count = arg_count;
    return n;
}

ASTNode *create_binary_node(const char *op, ASTNode *left, ASTNode *right, int line) {
    ASTNode *n = alloc_node(NODE_BINARY_EXPR, line);
    strncpy(n->data.binary_expr.op, op, 3);
    n->data.binary_expr.left = left;
    n->data.binary_expr.right = right;
    return n;
}

ASTNode *create_unary_node(const char *op, ASTNode *operand, int line) {
    ASTNode *n = alloc_node(NODE_UNARY_EXPR, line);
    strncpy(n->data.unary_expr.op, op, 3);
    n->data.unary_expr.operand = operand;
    return n;
}

ASTNode *create_assign_node(const char *name, ASTNode *value, int line) {
    ASTNode *n = alloc_node(NODE_ASSIGN_EXPR, line);
    n->data.assign_expr.name = safe_strdup(name);
    n->data.assign_expr.value = value;
    return n;
}

ASTNode *create_int_literal_node(int value, int line) {
    ASTNode *n = alloc_node(NODE_INT_LITERAL, line);
    n->data.int_literal.value = value;
    return n;
}

ASTNode *create_float_literal_node(double value, int line) {
    ASTNode *n = alloc_node(NODE_FLOAT_LITERAL, line);
    n->data.float_literal.value = value;
    return n;
}

ASTNode *create_char_literal_node(char value, int line) {
    ASTNode *n = alloc_node(NODE_CHAR_LITERAL, line);
    n->data.char_literal.value = value;
    return n;
}

ASTNode *create_string_literal_node(const char *value, int line) {
    ASTNode *n = alloc_node(NODE_STRING_LITERAL, line);
    n->data.string_literal.value = safe_strdup(value);
    return n;
}

ASTNode *create_identifier_node(const char *name, int line) {
    ASTNode *n = alloc_node(NODE_IDENTIFIER, line);
    n->data.identifier.name = safe_strdup(name);
    return n;
}

ASTNode *create_func_call_node(const char *name, ASTNode **args, int arg_count, int line) {
    ASTNode *n = alloc_node(NODE_FUNC_CALL, line);
    n->data.func_call.name = safe_strdup(name);
    n->data.func_call.args = args;
    n->data.func_call.arg_count = arg_count;
    return n;
}

/* ──────────────────────────────────────────────
 * Utility: Add declaration to program node
 * ────────────────────────────────────────────── */
void program_add_declaration(ASTNode *prog, ASTNode *decl) {
    if (prog->data.program.decl_count < MAX_CHILDREN) {
        prog->data.program.declarations[prog->data.program.decl_count++] = decl;
    }
}

/* ──────────────────────────────────────────────
 * Utility: Add item to compound statement
 * ────────────────────────────────────────────── */
void compound_add_item(ASTNode *compound, ASTNode *item) {
    if (compound->data.compound.item_count < MAX_CHILDREN) {
        compound->data.compound.items[compound->data.compound.item_count++] = item;
    }
}

/* ──────────────────────────────────────────────
 * Phase 2 Output: Print AST as indented tree
 *
 * Uses Unicode box-drawing characters for a
 * professional tree visualization:
 *   ├── for middle children
 *   └── for last child
 *   │   for continuation lines
 * ────────────────────────────────────────────── */
static void print_indent(int indent, int is_last, int *branch_stack) {
    for (int i = 0; i < indent; i++) {
        if (branch_stack[i])
            printf("|   ");
        else
            printf("    ");
    }
    if (indent >= 0) {
        if (is_last)
            printf("+-- ");
        else
            printf("|-- ");
    }
}

void print_ast_internal(ASTNode *node, int indent, int is_last, int *branch_stack) {
    if (!node) return;

    if (indent > 0) {
        print_indent(indent - 1, is_last, branch_stack);
    }

    switch (node->type) {
        case NODE_PROGRAM:
            printf("Program\n");
            for (int i = 0; i < node->data.program.decl_count; i++) {
                branch_stack[indent] = (i < node->data.program.decl_count - 1);
                print_ast_internal(node->data.program.declarations[i],
                                   indent + 1,
                                   i == node->data.program.decl_count - 1,
                                   branch_stack);
            }
            break;

        case NODE_VAR_DECL:
            printf("VarDecl: %s %s", node->data.var_decl.var_type, node->data.var_decl.name);
            if (node->data.var_decl.init_expr) {
                printf(" =\n");
                branch_stack[indent] = 0;
                print_ast_internal(node->data.var_decl.init_expr, indent + 1, 1, branch_stack);
            } else {
                printf("\n");
            }
            break;

        case NODE_FUN_DECL:
            printf("FuncDecl: %s %s(", node->data.fun_decl.return_type, node->data.fun_decl.name);
            for (int i = 0; i < node->data.fun_decl.param_count; i++) {
                if (i > 0) printf(", ");
                printf("%s %s",
                       node->data.fun_decl.params[i]->data.param.param_type,
                       node->data.fun_decl.params[i]->data.param.name);
            }
            printf(")\n");
            if (node->data.fun_decl.body) {
                branch_stack[indent] = 0;
                print_ast_internal(node->data.fun_decl.body, indent + 1, 1, branch_stack);
            }
            break;

        case NODE_PARAM:
            printf("Param: %s %s\n", node->data.param.param_type, node->data.param.name);
            break;

        case NODE_COMPOUND_STMT:
            printf("CompoundStmt\n");
            for (int i = 0; i < node->data.compound.item_count; i++) {
                branch_stack[indent] = (i < node->data.compound.item_count - 1);
                print_ast_internal(node->data.compound.items[i],
                                   indent + 1,
                                   i == node->data.compound.item_count - 1,
                                   branch_stack);
            }
            break;

        case NODE_IF_STMT:
            printf("IfStmt\n");
            branch_stack[indent] = (node->data.if_stmt.else_branch != NULL) ? 1 :
                                    (node->data.if_stmt.then_branch != NULL);
            printf("    ");
            for (int i = 0; i < indent; i++) {
                if (branch_stack[i]) printf("|   ");
                else printf("    ");
            }
            printf("|-- Condition:\n");
            branch_stack[indent] = 1;
            print_ast_internal(node->data.if_stmt.condition, indent + 2, 1, branch_stack);

            if (node->data.if_stmt.else_branch) {
                for (int i = 0; i < indent; i++) {
                    if (branch_stack[i]) printf("|   ");
                    else printf("    ");
                }
                printf("    |-- Then:\n");
                branch_stack[indent] = 1;
                print_ast_internal(node->data.if_stmt.then_branch, indent + 2, 1, branch_stack);

                for (int i = 0; i < indent; i++) {
                    if (branch_stack[i]) printf("|   ");
                    else printf("    ");
                }
                printf("    +-- Else:\n");
                branch_stack[indent] = 0;
                print_ast_internal(node->data.if_stmt.else_branch, indent + 2, 1, branch_stack);
            } else {
                for (int i = 0; i < indent; i++) {
                    if (branch_stack[i]) printf("|   ");
                    else printf("    ");
                }
                printf("    +-- Then:\n");
                branch_stack[indent] = 0;
                print_ast_internal(node->data.if_stmt.then_branch, indent + 2, 1, branch_stack);
            }
            break;

        case NODE_WHILE_STMT:
            printf("WhileStmt\n");
            branch_stack[indent] = 1;
            for (int i = 0; i < indent; i++) {
                if (branch_stack[i]) printf("|   ");
                else printf("    ");
            }
            printf("    |-- Condition:\n");
            print_ast_internal(node->data.while_stmt.condition, indent + 2, 1, branch_stack);
            branch_stack[indent] = 0;
            for (int i = 0; i < indent; i++) {
                if (branch_stack[i]) printf("|   ");
                else printf("    ");
            }
            printf("    +-- Body:\n");
            print_ast_internal(node->data.while_stmt.body, indent + 2, 1, branch_stack);
            break;

        case NODE_FOR_STMT:
            printf("ForStmt\n");
            branch_stack[indent] = 1;
            for (int i = 0; i < indent; i++) {
                if (branch_stack[i]) printf("|   ");
                else printf("    ");
            }
            printf("    |-- Init:\n");
            print_ast_internal(node->data.for_stmt.init, indent + 2, 1, branch_stack);
            for (int i = 0; i < indent; i++) {
                if (branch_stack[i]) printf("|   ");
                else printf("    ");
            }
            printf("    |-- Condition:\n");
            print_ast_internal(node->data.for_stmt.condition, indent + 2, 1, branch_stack);
            for (int i = 0; i < indent; i++) {
                if (branch_stack[i]) printf("|   ");
                else printf("    ");
            }
            printf("    |-- Update:\n");
            print_ast_internal(node->data.for_stmt.update, indent + 2, 1, branch_stack);
            branch_stack[indent] = 0;
            for (int i = 0; i < indent; i++) {
                if (branch_stack[i]) printf("|   ");
                else printf("    ");
            }
            printf("    +-- Body:\n");
            print_ast_internal(node->data.for_stmt.body, indent + 2, 1, branch_stack);
            break;

        case NODE_RETURN_STMT:
            printf("ReturnStmt\n");
            if (node->data.return_stmt.expr) {
                branch_stack[indent] = 0;
                print_ast_internal(node->data.return_stmt.expr, indent + 1, 1, branch_stack);
            }
            break;

        case NODE_EXPR_STMT:
            printf("ExprStmt\n");
            if (node->data.expr_stmt.expr) {
                branch_stack[indent] = 0;
                print_ast_internal(node->data.expr_stmt.expr, indent + 1, 1, branch_stack);
            }
            break;

        case NODE_PRINTF_STMT:
            printf("PrintfStmt: \"%s\"\n", node->data.printf_stmt.format_str);
            for (int i = 0; i < node->data.printf_stmt.arg_count; i++) {
                branch_stack[indent] = (i < node->data.printf_stmt.arg_count - 1);
                print_ast_internal(node->data.printf_stmt.args[i],
                                   indent + 1,
                                   i == node->data.printf_stmt.arg_count - 1,
                                   branch_stack);
            }
            break;

        case NODE_BINARY_EXPR:
            printf("BinaryExpr: %s\n", node->data.binary_expr.op);
            branch_stack[indent] = 1;
            print_ast_internal(node->data.binary_expr.left, indent + 1, 0, branch_stack);
            branch_stack[indent] = 0;
            print_ast_internal(node->data.binary_expr.right, indent + 1, 1, branch_stack);
            break;

        case NODE_UNARY_EXPR:
            printf("UnaryExpr: %s\n", node->data.unary_expr.op);
            branch_stack[indent] = 0;
            print_ast_internal(node->data.unary_expr.operand, indent + 1, 1, branch_stack);
            break;

        case NODE_ASSIGN_EXPR:
            printf("AssignExpr: %s =\n", node->data.assign_expr.name);
            branch_stack[indent] = 0;
            print_ast_internal(node->data.assign_expr.value, indent + 1, 1, branch_stack);
            break;

        case NODE_INT_LITERAL:
            printf("IntLiteral: %d\n", node->data.int_literal.value);
            break;

        case NODE_FLOAT_LITERAL:
            printf("FloatLiteral: %.2f\n", node->data.float_literal.value);
            break;

        case NODE_CHAR_LITERAL:
            printf("CharLiteral: '%c'\n", node->data.char_literal.value);
            break;

        case NODE_STRING_LITERAL:
            printf("StringLiteral: \"%s\"\n", node->data.string_literal.value);
            break;

        case NODE_IDENTIFIER:
            printf("Identifier: %s\n", node->data.identifier.name);
            break;

        case NODE_FUNC_CALL:
            printf("FuncCall: %s()\n", node->data.func_call.name);
            for (int i = 0; i < node->data.func_call.arg_count; i++) {
                branch_stack[indent] = (i < node->data.func_call.arg_count - 1);
                print_ast_internal(node->data.func_call.args[i],
                                   indent + 1,
                                   i == node->data.func_call.arg_count - 1,
                                   branch_stack);
            }
            break;
    }
}

/* Public entry point for AST printing */
void print_ast(ASTNode *node, int indent) {
    int branch_stack[128] = {0};
    print_ast_internal(node, indent, 1, branch_stack);
}

/* ──────────────────────────────────────────────
 * Free all memory used by AST
 * ────────────────────────────────────────────── */
void free_ast(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            for (int i = 0; i < node->data.program.decl_count; i++)
                free_ast(node->data.program.declarations[i]);
            free(node->data.program.declarations);
            break;
        case NODE_VAR_DECL:
            free(node->data.var_decl.var_type);
            free(node->data.var_decl.name);
            free_ast(node->data.var_decl.init_expr);
            break;
        case NODE_FUN_DECL:
            free(node->data.fun_decl.return_type);
            free(node->data.fun_decl.name);
            for (int i = 0; i < node->data.fun_decl.param_count; i++)
                free_ast(node->data.fun_decl.params[i]);
            free(node->data.fun_decl.params);
            free_ast(node->data.fun_decl.body);
            break;
        case NODE_PARAM:
            free(node->data.param.param_type);
            free(node->data.param.name);
            break;
        case NODE_COMPOUND_STMT:
            for (int i = 0; i < node->data.compound.item_count; i++)
                free_ast(node->data.compound.items[i]);
            free(node->data.compound.items);
            break;
        case NODE_IF_STMT:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.then_branch);
            free_ast(node->data.if_stmt.else_branch);
            break;
        case NODE_WHILE_STMT:
            free_ast(node->data.while_stmt.condition);
            free_ast(node->data.while_stmt.body);
            break;
        case NODE_FOR_STMT:
            free_ast(node->data.for_stmt.init);
            free_ast(node->data.for_stmt.condition);
            free_ast(node->data.for_stmt.update);
            free_ast(node->data.for_stmt.body);
            break;
        case NODE_RETURN_STMT:
            free_ast(node->data.return_stmt.expr);
            break;
        case NODE_EXPR_STMT:
            free_ast(node->data.expr_stmt.expr);
            break;
        case NODE_PRINTF_STMT:
            free(node->data.printf_stmt.format_str);
            for (int i = 0; i < node->data.printf_stmt.arg_count; i++)
                free_ast(node->data.printf_stmt.args[i]);
            free(node->data.printf_stmt.args);
            break;
        case NODE_BINARY_EXPR:
            free_ast(node->data.binary_expr.left);
            free_ast(node->data.binary_expr.right);
            break;
        case NODE_UNARY_EXPR:
            free_ast(node->data.unary_expr.operand);
            break;
        case NODE_ASSIGN_EXPR:
            free(node->data.assign_expr.name);
            free_ast(node->data.assign_expr.value);
            break;
        case NODE_STRING_LITERAL:
            free(node->data.string_literal.value);
            break;
        case NODE_IDENTIFIER:
            free(node->data.identifier.name);
            break;
        case NODE_FUNC_CALL:
            free(node->data.func_call.name);
            for (int i = 0; i < node->data.func_call.arg_count; i++)
                free_ast(node->data.func_call.args[i]);
            free(node->data.func_call.args);
            break;
        default:
            break;
    }
    free(node);
}
