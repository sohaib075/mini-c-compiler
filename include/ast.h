/*
 * ============================================================
 *   MINI-C COMPILER — Abstract Syntax Tree (AST)
 *   File: ast.h
 *   Purpose: Defines all AST node types for the Mini-C
 *            language. Used by the parser to build the tree
 *            and by all subsequent phases to traverse it.
 * ============================================================
 */

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ──────────────────────────────────────────────
 * Maximum limits for compiler data structures
 * ────────────────────────────────────────────── */
#define MAX_CHILDREN  256
#define MAX_PARAMS     32
#define MAX_ARGS       32

/* ──────────────────────────────────────────────
 * AST Node Types — Every construct in Mini-C
 * maps to one of these node types
 * ────────────────────────────────────────────── */
typedef enum {
    NODE_PROGRAM,          /* Root: contains list of declarations       */
    NODE_VAR_DECL,         /* Variable declaration: int x; or int x=5; */
    NODE_FUN_DECL,         /* Function declaration: int main() {...}   */
    NODE_PARAM,            /* Function parameter: int x                */
    NODE_COMPOUND_STMT,    /* Block: { ... }                           */
    NODE_IF_STMT,          /* if (cond) {...} else {...}               */
    NODE_WHILE_STMT,       /* while (cond) {...}                       */
    NODE_FOR_STMT,         /* for (init; cond; update) {...}           */
    NODE_RETURN_STMT,      /* return expr;                             */
    NODE_EXPR_STMT,        /* Expression statement: expr;              */
    NODE_PRINTF_STMT,      /* printf("...", args);                     */
    NODE_BINARY_EXPR,      /* Binary expression: a + b, a < b, etc.   */
    NODE_UNARY_EXPR,       /* Unary expression: -a, !a                */
    NODE_ASSIGN_EXPR,      /* Assignment: x = expr                    */
    NODE_INT_LITERAL,      /* Integer constant: 42                    */
    NODE_FLOAT_LITERAL,    /* Float constant: 3.14                    */
    NODE_CHAR_LITERAL,     /* Character constant: 'a'                 */
    NODE_STRING_LITERAL,   /* String constant: "hello"                */
    NODE_IDENTIFIER,       /* Variable reference: x                   */
    NODE_FUNC_CALL,        /* Function call: foo(a, b)                */
} NodeType;

/* ──────────────────────────────────────────────
 * AST Node Structure — A tagged union
 * Each node has a type, line number, and
 * type-specific data stored in a union.
 * ────────────────────────────────────────────── */
typedef struct ASTNode {
    NodeType type;
    int line_number;

    union {
        /* Program: list of top-level declarations */
        struct {
            struct ASTNode **declarations;
            int decl_count;
        } program;

        /* Variable declaration: type name [= init_expr]; */
        struct {
            char *var_type;
            char *name;
            struct ASTNode *init_expr;   /* NULL if no initializer */
        } var_decl;

        /* Function declaration: ret_type name(params) { body } */
        struct {
            char *return_type;
            char *name;
            struct ASTNode **params;
            int param_count;
            struct ASTNode *body;        /* CompoundStmt */
        } fun_decl;

        /* Parameter: type name */
        struct {
            char *param_type;
            char *name;
        } param;

        /* Compound statement (block): { items... } */
        struct {
            struct ASTNode **items;      /* Mix of decls and stmts */
            int item_count;
        } compound;

        /* If statement: if (cond) then [else else_branch] */
        struct {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch; /* NULL if no else */
        } if_stmt;

        /* While loop: while (cond) body */
        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_stmt;

        /* For loop: for (init; cond; update) body */
        struct {
            struct ASTNode *init;
            struct ASTNode *condition;
            struct ASTNode *update;
            struct ASTNode *body;
        } for_stmt;

        /* Return statement: return [expr]; */
        struct {
            struct ASTNode *expr;        /* NULL for bare return */
        } return_stmt;

        /* Expression statement: expr; */
        struct {
            struct ASTNode *expr;
        } expr_stmt;

        /* Printf statement: printf(fmt, args...) */
        struct {
            char *format_str;
            struct ASTNode **args;
            int arg_count;
        } printf_stmt;

        /* Binary expression: left OP right */
        struct {
            char op[4];                  /* "+", "-", "==", "&&", etc. */
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_expr;

        /* Unary expression: OP operand */
        struct {
            char op[4];                  /* "-", "!" */
            struct ASTNode *operand;
        } unary_expr;

        /* Assignment expression: name = value */
        struct {
            char *name;
            struct ASTNode *value;
        } assign_expr;

        /* Integer literal */
        struct { int value; } int_literal;

        /* Float literal */
        struct { double value; } float_literal;

        /* Char literal */
        struct { char value; } char_literal;

        /* String literal */
        struct { char *value; } string_literal;

        /* Identifier (variable reference) */
        struct { char *name; } identifier;

        /* Function call: name(args...) */
        struct {
            char *name;
            struct ASTNode **args;
            int arg_count;
        } func_call;
    } data;
} ASTNode;

/* ──────────────────────────────────────────────
 * AST Constructor Functions
 * Each creates a new node of the specified type
 * ────────────────────────────────────────────── */
ASTNode *create_program_node(int line);
ASTNode *create_var_decl_node(const char *type, const char *name, ASTNode *init, int line);
ASTNode *create_fun_decl_node(const char *ret_type, const char *name, ASTNode **params, int param_count, ASTNode *body, int line);
ASTNode *create_param_node(const char *type, const char *name, int line);
ASTNode *create_compound_node(int line);
ASTNode *create_if_node(ASTNode *cond, ASTNode *then_b, ASTNode *else_b, int line);
ASTNode *create_while_node(ASTNode *cond, ASTNode *body, int line);
ASTNode *create_for_node(ASTNode *init, ASTNode *cond, ASTNode *update, ASTNode *body, int line);
ASTNode *create_return_node(ASTNode *expr, int line);
ASTNode *create_expr_stmt_node(ASTNode *expr, int line);
ASTNode *create_printf_node(const char *fmt, ASTNode **args, int arg_count, int line);
ASTNode *create_binary_node(const char *op, ASTNode *left, ASTNode *right, int line);
ASTNode *create_unary_node(const char *op, ASTNode *operand, int line);
ASTNode *create_assign_node(const char *name, ASTNode *value, int line);
ASTNode *create_int_literal_node(int value, int line);
ASTNode *create_float_literal_node(double value, int line);
ASTNode *create_char_literal_node(char value, int line);
ASTNode *create_string_literal_node(const char *value, int line);
ASTNode *create_identifier_node(const char *name, int line);
ASTNode *create_func_call_node(const char *name, ASTNode **args, int arg_count, int line);

/* ── Utility Functions ── */
void program_add_declaration(ASTNode *prog, ASTNode *decl);
void compound_add_item(ASTNode *compound, ASTNode *item);
void print_ast(ASTNode *node, int indent);
void free_ast(ASTNode *node);

#endif /* AST_H */
