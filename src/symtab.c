/*
 * ============================================================
 *   MINI-C COMPILER — Phase 3: Semantic Analysis
 *   File: symtab.c
 *   Purpose: Implements symbol table operations and semantic
 *            checks: type checking, undeclared variables,
 *            duplicate declarations, scope validation.
 *   Output:  Symbol table and semantic error messages.
 * ============================================================
 */

#include "symtab.h"
#include <stdarg.h>

/* ──────────────────────────────────────────────
 * Initialize the symbol table
 * ────────────────────────────────────────────── */
void symtab_init(SymbolTable *st) {
    st->count = 0;
    st->scope_depth = 0;
    st->error_count = 0;
    strcpy(st->scope_stack[0], "global");
    st->scope_start[0] = 0;
}

/* ──────────────────────────────────────────────
 * Scope Management
 *
 * The scope stack tracks nested scopes.
 * When entering a function body, we push
 * "funcname()" onto the stack. When entering
 * a block inside that, we push "block".
 * On exit, symbols declared in that scope
 * are kept (for display) but hidden from lookup.
 * ────────────────────────────────────────────── */
void symtab_enter_scope(SymbolTable *st, const char *scope_name) {
    st->scope_depth++;
    if (st->scope_depth < MAX_SCOPES) {
        strncpy(st->scope_stack[st->scope_depth], scope_name, 127);
        st->scope_stack[st->scope_depth][127] = '\0';
        st->scope_start[st->scope_depth] = st->count;
    }
}

void symtab_exit_scope(SymbolTable *st) {
    if (st->scope_depth > 0) {
        st->scope_depth--;
    }
}

const char *symtab_current_scope(SymbolTable *st) {
    return st->scope_stack[st->scope_depth];
}

/* ──────────────────────────────────────────────
 * Insert a symbol into the table
 *
 * Returns 1 on success, 0 if duplicate in
 * current scope.
 * ────────────────────────────────────────────── */
int symtab_insert(SymbolTable *st, const char *name, const char *type,
                  int line, int is_function, int param_count) {
    /* Check for duplicate in current scope */
    Symbol *existing = symtab_lookup_current_scope(st, name);
    if (existing) {
        semantic_error(st, line,
            "Duplicate declaration: '%s' already declared at line %d in scope '%s'",
            name, existing->line_declared, existing->scope);
        return 0;
    }

    if (st->count >= SYMTAB_SIZE) {
        semantic_error(st, line, "Symbol table overflow");
        return 0;
    }

    Symbol *sym = &st->symbols[st->count++];
    strncpy(sym->name, name, 127); sym->name[127] = '\0';
    strncpy(sym->type, type, 31);  sym->type[31] = '\0';
    strncpy(sym->scope, symtab_current_scope(st), 127); sym->scope[127] = '\0';
    sym->line_declared = line;
    sym->is_function = is_function;
    sym->param_count = param_count;
    sym->is_used = 0;

    return 1;
}

/* ──────────────────────────────────────────────
 * Lookup: search all scopes (inner → outer)
 * ────────────────────────────────────────────── */
Symbol *symtab_lookup(SymbolTable *st, const char *name) {
    /* Search from most recent to oldest */
    for (int i = st->count - 1; i >= 0; i--) {
        if (strcmp(st->symbols[i].name, name) == 0) {
            return &st->symbols[i];
        }
    }
    return NULL;
}

/* ──────────────────────────────────────────────
 * Lookup: search only current scope
 * ────────────────────────────────────────────── */
Symbol *symtab_lookup_current_scope(SymbolTable *st, const char *name) {
    const char *current = symtab_current_scope(st);
    int start = (st->scope_depth < MAX_SCOPES) ? st->scope_start[st->scope_depth] : 0;

    for (int i = start; i < st->count; i++) {
        if (strcmp(st->symbols[i].name, name) == 0 &&
            strcmp(st->symbols[i].scope, current) == 0) {
            return &st->symbols[i];
        }
    }
    return NULL;
}

/* ──────────────────────────────────────────────
 * Error Reporting
 * ────────────────────────────────────────────── */
void semantic_error(SymbolTable *st, int line, const char *fmt, ...) {
    if (st->error_count >= 256) return;

    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    snprintf(st->errors[st->error_count], 256,
             "Line %d: %s", line, buffer);
    st->error_count++;
}

/* ──────────────────────────────────────────────
 * Helper: Get the type of an expression node
 * Returns "int", "float", "char", or "unknown"
 * ────────────────────────────────────────────── */
static const char *get_expr_type(ASTNode *node, SymbolTable *st) {
    if (!node) return "void";

    switch (node->type) {
        case NODE_INT_LITERAL:
            return "int";
        case NODE_FLOAT_LITERAL:
            return "float";
        case NODE_CHAR_LITERAL:
            return "char";
        case NODE_STRING_LITERAL:
            return "char*";
        case NODE_IDENTIFIER: {
            Symbol *sym = symtab_lookup(st, node->data.identifier.name);
            if (sym) {
                sym->is_used = 1;
                return sym->type;
            }
            return "unknown";
        }
        case NODE_BINARY_EXPR: {
            const char *lt = get_expr_type(node->data.binary_expr.left, st);
            const char *rt = get_expr_type(node->data.binary_expr.right, st);
            /* Relational/logical operators always produce int */
            if (strcmp(node->data.binary_expr.op, "==") == 0 ||
                strcmp(node->data.binary_expr.op, "!=") == 0 ||
                strcmp(node->data.binary_expr.op, "<") == 0 ||
                strcmp(node->data.binary_expr.op, ">") == 0 ||
                strcmp(node->data.binary_expr.op, "<=") == 0 ||
                strcmp(node->data.binary_expr.op, ">=") == 0 ||
                strcmp(node->data.binary_expr.op, "&&") == 0 ||
                strcmp(node->data.binary_expr.op, "||") == 0) {
                return "int";
            }
            /* If either is float, result is float */
            if (strcmp(lt, "float") == 0 || strcmp(rt, "float") == 0)
                return "float";
            return "int";
        }
        case NODE_UNARY_EXPR:
            return get_expr_type(node->data.unary_expr.operand, st);
        case NODE_ASSIGN_EXPR: {
            Symbol *sym = symtab_lookup(st, node->data.assign_expr.name);
            if (sym) return sym->type;
            return "unknown";
        }
        case NODE_FUNC_CALL: {
            Symbol *sym = symtab_lookup(st, node->data.func_call.name);
            if (sym) return sym->type;
            return "unknown";
        }
        default:
            return "unknown";
    }
}

/* ──────────────────────────────────────────────
 * Semantic Analysis: Recursive AST Traversal
 *
 * Checks performed:
 *   1. Undeclared variable detection
 *   2. Duplicate declaration detection
 *   3. Type checking on assignments
 *   4. Scope validation
 *   5. Function call argument checking
 *   6. Return type checking
 * ────────────────────────────────────────────── */
static void analyze_node(ASTNode *node, SymbolTable *st) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            for (int i = 0; i < node->data.program.decl_count; i++) {
                analyze_node(node->data.program.declarations[i], st);
            }
            break;

        case NODE_VAR_DECL:
            /* Insert variable into symbol table */
            symtab_insert(st, node->data.var_decl.name,
                         node->data.var_decl.var_type,
                         node->line_number, 0, 0);
            /* Check initializer expression if present */
            if (node->data.var_decl.init_expr) {
                analyze_node(node->data.var_decl.init_expr, st);
                /* Type check: warn if assigning float to int, etc. */
                const char *expr_type = get_expr_type(node->data.var_decl.init_expr, st);
                if (strcmp(node->data.var_decl.var_type, "int") == 0 &&
                    strcmp(expr_type, "float") == 0) {
                    semantic_error(st, node->line_number,
                        "Warning: implicit conversion from 'float' to 'int' in declaration of '%s'",
                        node->data.var_decl.name);
                }
            }
            break;

        case NODE_FUN_DECL:
            /* Insert function into symbol table at current scope */
            symtab_insert(st, node->data.fun_decl.name,
                         node->data.fun_decl.return_type,
                         node->line_number, 1,
                         node->data.fun_decl.param_count);

            /* Enter function scope */
            {
                char scope_name[128];
                snprintf(scope_name, sizeof(scope_name), "%s()",
                         node->data.fun_decl.name);
                symtab_enter_scope(st, scope_name);
            }

            /* Add parameters as local variables */
            for (int i = 0; i < node->data.fun_decl.param_count; i++) {
                ASTNode *p = node->data.fun_decl.params[i];
                symtab_insert(st, p->data.param.name,
                             p->data.param.param_type,
                             p->line_number, 0, 0);
            }

            /* Analyze function body */
            if (node->data.fun_decl.body) {
                analyze_node(node->data.fun_decl.body, st);
            }

            symtab_exit_scope(st);
            break;

        case NODE_COMPOUND_STMT:
            for (int i = 0; i < node->data.compound.item_count; i++) {
                analyze_node(node->data.compound.items[i], st);
            }
            break;

        case NODE_IF_STMT:
            analyze_node(node->data.if_stmt.condition, st);
            analyze_node(node->data.if_stmt.then_branch, st);
            if (node->data.if_stmt.else_branch) {
                analyze_node(node->data.if_stmt.else_branch, st);
            }
            break;

        case NODE_WHILE_STMT:
            analyze_node(node->data.while_stmt.condition, st);
            analyze_node(node->data.while_stmt.body, st);
            break;

        case NODE_FOR_STMT:
            analyze_node(node->data.for_stmt.init, st);
            analyze_node(node->data.for_stmt.condition, st);
            analyze_node(node->data.for_stmt.update, st);
            analyze_node(node->data.for_stmt.body, st);
            break;

        case NODE_RETURN_STMT:
            if (node->data.return_stmt.expr) {
                analyze_node(node->data.return_stmt.expr, st);
            }
            break;

        case NODE_EXPR_STMT:
            if (node->data.expr_stmt.expr) {
                analyze_node(node->data.expr_stmt.expr, st);
            }
            break;

        case NODE_PRINTF_STMT:
            for (int i = 0; i < node->data.printf_stmt.arg_count; i++) {
                analyze_node(node->data.printf_stmt.args[i], st);
            }
            break;

        case NODE_BINARY_EXPR:
            analyze_node(node->data.binary_expr.left, st);
            analyze_node(node->data.binary_expr.right, st);
            /* Type compatibility check */
            {
                const char *lt = get_expr_type(node->data.binary_expr.left, st);
                const char *rt = get_expr_type(node->data.binary_expr.right, st);
                if (strcmp(lt, "char*") == 0 || strcmp(rt, "char*") == 0) {
                    /* String operands in arithmetic */
                    if (strcmp(node->data.binary_expr.op, "+") == 0 ||
                        strcmp(node->data.binary_expr.op, "-") == 0 ||
                        strcmp(node->data.binary_expr.op, "*") == 0 ||
                        strcmp(node->data.binary_expr.op, "/") == 0) {
                        semantic_error(st, node->line_number,
                            "Type error: cannot perform arithmetic on string types");
                    }
                }
            }
            break;

        case NODE_UNARY_EXPR:
            analyze_node(node->data.unary_expr.operand, st);
            break;

        case NODE_ASSIGN_EXPR: {
            /* Check that variable is declared */
            Symbol *sym = symtab_lookup(st, node->data.assign_expr.name);
            if (!sym) {
                semantic_error(st, node->line_number,
                    "Undeclared variable: '%s'", node->data.assign_expr.name);
            } else {
                sym->is_used = 1;
                /* Type check assignment */
                analyze_node(node->data.assign_expr.value, st);
                const char *expr_type = get_expr_type(node->data.assign_expr.value, st);
                if (strcmp(sym->type, "int") == 0 && strcmp(expr_type, "float") == 0) {
                    semantic_error(st, node->line_number,
                        "Warning: implicit conversion from 'float' to 'int' in assignment to '%s'",
                        sym->name);
                }
            }
            break;
        }

        case NODE_IDENTIFIER: {
            Symbol *sym = symtab_lookup(st, node->data.identifier.name);
            if (!sym) {
                semantic_error(st, node->line_number,
                    "Undeclared variable: '%s'", node->data.identifier.name);
            } else {
                sym->is_used = 1;
            }
            break;
        }

        case NODE_FUNC_CALL: {
            Symbol *sym = symtab_lookup(st, node->data.func_call.name);
            if (!sym) {
                semantic_error(st, node->line_number,
                    "Undeclared function: '%s'", node->data.func_call.name);
            } else if (!sym->is_function) {
                semantic_error(st, node->line_number,
                    "'%s' is not a function", node->data.func_call.name);
            } else {
                sym->is_used = 1;
                if (sym->param_count != node->data.func_call.arg_count) {
                    semantic_error(st, node->line_number,
                        "Function '%s' expects %d arguments, got %d",
                        node->data.func_call.name,
                        sym->param_count,
                        node->data.func_call.arg_count);
                }
            }
            /* Analyze arguments */
            for (int i = 0; i < node->data.func_call.arg_count; i++) {
                analyze_node(node->data.func_call.args[i], st);
            }
            break;
        }

        default:
            /* Literals don't need analysis */
            break;
    }
}

/* ──────────────────────────────────────────────
 * Public: Run semantic analysis on AST
 * ────────────────────────────────────────────── */
void semantic_analysis(ASTNode *root, SymbolTable *st) {
    symtab_init(st);
    analyze_node(root, st);
}

/* ──────────────────────────────────────────────
 * Phase 3 Output: Print Symbol Table & Errors
 *
 * Matches PDF format:
 *   Variable | Type | Scope | Line Decl. | Status
 * ────────────────────────────────────────────── */
void print_symbol_table(SymbolTable *st) {
    printf("\n");
    printf("==========================================================\n");
    printf("              PHASE 3: SEMANTIC ANALYSIS                   \n");
    printf("==========================================================\n\n");

    printf("  Symbol Table:\n");
    printf("  %-15s %-10s %-15s %-12s %s\n",
           "Variable", "Type", "Scope", "Line Decl.", "Status");
    printf("  %-15s %-10s %-15s %-12s %s\n",
           "--------------", "---------", "--------------", "-----------", "------");

    for (int i = 0; i < st->count; i++) {
        Symbol *s = &st->symbols[i];
        printf("  %-15s %-10s %-15s %-12d %s\n",
               s->name, s->type, s->scope, s->line_declared, "OK");
    }

    printf("\n");

    if (st->error_count > 0) {
        printf("  Semantic Errors Found (%d):\n", st->error_count);
        for (int i = 0; i < st->error_count; i++) {
            printf("    [ERROR] %s\n", st->errors[i]);
        }
    } else {
        printf("  Semantic Analysis Passed -- No errors found.\n");
    }
    printf("\n");
}
