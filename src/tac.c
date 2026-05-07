/*
 * ============================================================
 *   MINI-C COMPILER — Phase 4: Intermediate Code Generation
 *   File: tac.c
 *   Purpose: Converts the validated AST into Three-Address
 *            Code (TAC) using temporary variables and labels.
 *   Output:  Numbered TAC instruction list.
 * ============================================================
 */

#include "tac.h"

/* ──────────────────────────────────────────────
 * Initialize TAC list
 * ────────────────────────────────────────────── */
void tac_init(TACList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->instr_count = 0;
    list->temp_count = 0;
    list->label_count = 0;
}

/* ──────────────────────────────────────────────
 * Emit a new TAC instruction at the end
 * ────────────────────────────────────────────── */
TACInstr *tac_emit(TACList *list, TACOp op, const char *result,
                    const char *arg1, const char *arg2) {
    TACInstr *instr = (TACInstr *)calloc(1, sizeof(TACInstr));
    if (!instr) { fprintf(stderr, "TAC: Memory allocation failed\n"); exit(1); }

    instr->op = op;
    instr->index = list->instr_count++;
    if (result) strncpy(instr->result, result, 63);
    if (arg1)   strncpy(instr->arg1, arg1, 63);
    if (arg2)   strncpy(instr->arg2, arg2, 63);
    instr->next = NULL;

    if (!list->head) {
        list->head = list->tail = instr;
    } else {
        list->tail->next = instr;
        list->tail = instr;
    }
    return instr;
}

/* ──────────────────────────────────────────────
 * Generate a fresh temporary variable name
 * Returns: "t0", "t1", "t2", ...
 * ────────────────────────────────────────────── */
char *tac_new_temp(TACList *list) {
    char *temp = (char *)malloc(16);
    sprintf(temp, "t%d", list->temp_count++);
    return temp;
}

/* ──────────────────────────────────────────────
 * Generate a fresh label name
 * Returns: "L0", "L1", "L2", ...
 * ────────────────────────────────────────────── */
char *tac_new_label(TACList *list) {
    char *label = (char *)malloc(16);
    sprintf(label, "L%d", list->label_count++);
    return label;
}

/* ──────────────────────────────────────────────
 * Helper: Convert int to string
 * ────────────────────────────────────────────── */
static char *int_to_str(int val) {
    char *s = (char *)malloc(32);
    sprintf(s, "%d", val);
    return s;
}

static char *float_to_str(double val) {
    char *s = (char *)malloc(32);
    sprintf(s, "%.2f", val);
    return s;
}

/* ──────────────────────────────────────────────
 * Map AST binary operator to TAC operation
 * ────────────────────────────────────────────── */
static TACOp op_to_tac(const char *op) {
    if (strcmp(op, "+") == 0)  return TAC_ADD;
    if (strcmp(op, "-") == 0)  return TAC_SUB;
    if (strcmp(op, "*") == 0)  return TAC_MUL;
    if (strcmp(op, "/") == 0)  return TAC_DIV;
    if (strcmp(op, "%") == 0)  return TAC_MOD;
    if (strcmp(op, "==") == 0) return TAC_EQ;
    if (strcmp(op, "!=") == 0) return TAC_NEQ;
    if (strcmp(op, "<") == 0)  return TAC_LT;
    if (strcmp(op, ">") == 0)  return TAC_GT;
    if (strcmp(op, "<=") == 0) return TAC_LTE;
    if (strcmp(op, ">=") == 0) return TAC_GTE;
    if (strcmp(op, "&&") == 0) return TAC_AND;
    if (strcmp(op, "||") == 0) return TAC_OR;
    return TAC_ADD; /* fallback */
}

/* ──────────────────────────────────────────────
 * Generate TAC for an expression
 *
 * Returns the name of the temporary (or variable)
 * that holds the result of the expression.
 * ────────────────────────────────────────────── */
static char *gen_expr(ASTNode *node, TACList *list) {
    if (!node) return strdup("0");

    switch (node->type) {
        case NODE_INT_LITERAL: {
            char *temp = tac_new_temp(list);
            char *val = int_to_str(node->data.int_literal.value);
            tac_emit(list, TAC_COPY, temp, val, NULL);
            free(val);
            return temp;
        }

        case NODE_FLOAT_LITERAL: {
            char *temp = tac_new_temp(list);
            char *val = float_to_str(node->data.float_literal.value);
            tac_emit(list, TAC_COPY, temp, val, NULL);
            free(val);
            return temp;
        }

        case NODE_CHAR_LITERAL: {
            char *temp = tac_new_temp(list);
            char val[8];
            sprintf(val, "'%c'", node->data.char_literal.value);
            tac_emit(list, TAC_COPY, temp, val, NULL);
            return temp;
        }

        case NODE_STRING_LITERAL: {
            return strdup(node->data.string_literal.value);
        }

        case NODE_IDENTIFIER: {
            return strdup(node->data.identifier.name);
        }

        case NODE_BINARY_EXPR: {
            char *left = gen_expr(node->data.binary_expr.left, list);
            char *right = gen_expr(node->data.binary_expr.right, list);
            char *temp = tac_new_temp(list);
            TACOp op = op_to_tac(node->data.binary_expr.op);
            tac_emit(list, op, temp, left, right);
            free(left); free(right);
            return temp;
        }

        case NODE_UNARY_EXPR: {
            char *operand = gen_expr(node->data.unary_expr.operand, list);
            char *temp = tac_new_temp(list);
            if (strcmp(node->data.unary_expr.op, "-") == 0) {
                tac_emit(list, TAC_NEG, temp, operand, NULL);
            } else if (strcmp(node->data.unary_expr.op, "!") == 0) {
                tac_emit(list, TAC_NOT, temp, operand, NULL);
            }
            free(operand);
            return temp;
        }

        case NODE_ASSIGN_EXPR: {
            char *val = gen_expr(node->data.assign_expr.value, list);
            tac_emit(list, TAC_COPY, node->data.assign_expr.name, val, NULL);
            free(val);
            return strdup(node->data.assign_expr.name);
        }

        case NODE_FUNC_CALL: {
            /* Push arguments */
            for (int i = 0; i < node->data.func_call.arg_count; i++) {
                char *arg = gen_expr(node->data.func_call.args[i], list);
                tac_emit(list, TAC_PARAM, NULL, arg, NULL);
                free(arg);
            }
            char *temp = tac_new_temp(list);
            char count_str[16];
            sprintf(count_str, "%d", node->data.func_call.arg_count);
            tac_emit(list, TAC_CALL, temp, node->data.func_call.name, count_str);
            return temp;
        }

        default:
            return strdup("0");
    }
}

/* ──────────────────────────────────────────────
 * Generate TAC for a statement
 * ────────────────────────────────────────────── */
static void gen_stmt(ASTNode *node, TACList *list) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            for (int i = 0; i < node->data.program.decl_count; i++) {
                gen_stmt(node->data.program.declarations[i], list);
            }
            break;

        case NODE_FUN_DECL:
            tac_emit(list, TAC_FUNC_BEGIN, NULL, node->data.fun_decl.name, NULL);
            if (node->data.fun_decl.body) {
                gen_stmt(node->data.fun_decl.body, list);
            }
            tac_emit(list, TAC_FUNC_END, NULL, node->data.fun_decl.name, NULL);
            break;

        case NODE_VAR_DECL:
            if (node->data.var_decl.init_expr) {
                char *val = gen_expr(node->data.var_decl.init_expr, list);
                tac_emit(list, TAC_COPY, node->data.var_decl.name, val, NULL);
                free(val);
            }
            break;

        case NODE_COMPOUND_STMT:
            for (int i = 0; i < node->data.compound.item_count; i++) {
                gen_stmt(node->data.compound.items[i], list);
            }
            break;

        case NODE_EXPR_STMT:
            if (node->data.expr_stmt.expr) {
                char *result = gen_expr(node->data.expr_stmt.expr, list);
                free(result);
            }
            break;

        case NODE_IF_STMT: {
            /*
             * if (cond) then_body else else_body
             *
             * TAC:
             *   <eval cond into t>
             *   ifFalse t goto L_else
             *   <then_body>
             *   goto L_end
             * L_else:
             *   <else_body>
             * L_end:
             */
            char *cond = gen_expr(node->data.if_stmt.condition, list);

            if (node->data.if_stmt.else_branch) {
                char *l_else = tac_new_label(list);
                char *l_end = tac_new_label(list);

                tac_emit(list, TAC_IF_FALSE_GOTO, l_else, cond, NULL);
                gen_stmt(node->data.if_stmt.then_branch, list);
                tac_emit(list, TAC_GOTO, l_end, NULL, NULL);
                tac_emit(list, TAC_LABEL, l_else, NULL, NULL);
                gen_stmt(node->data.if_stmt.else_branch, list);
                tac_emit(list, TAC_LABEL, l_end, NULL, NULL);

                free(l_else); free(l_end);
            } else {
                char *l_end = tac_new_label(list);
                tac_emit(list, TAC_IF_FALSE_GOTO, l_end, cond, NULL);
                gen_stmt(node->data.if_stmt.then_branch, list);
                tac_emit(list, TAC_LABEL, l_end, NULL, NULL);
                free(l_end);
            }
            free(cond);
            break;
        }

        case NODE_WHILE_STMT: {
            /*
             * while (cond) body
             *
             * TAC:
             * L_start:
             *   <eval cond into t>
             *   ifFalse t goto L_end
             *   <body>
             *   goto L_start
             * L_end:
             */
            char *l_start = tac_new_label(list);
            char *l_end = tac_new_label(list);

            tac_emit(list, TAC_LABEL, l_start, NULL, NULL);
            char *cond = gen_expr(node->data.while_stmt.condition, list);
            tac_emit(list, TAC_IF_FALSE_GOTO, l_end, cond, NULL);
            gen_stmt(node->data.while_stmt.body, list);
            tac_emit(list, TAC_GOTO, l_start, NULL, NULL);
            tac_emit(list, TAC_LABEL, l_end, NULL, NULL);

            free(l_start); free(l_end); free(cond);
            break;
        }

        case NODE_FOR_STMT: {
            /*
             * for (init; cond; update) body
             *
             * TAC:
             *   <init>
             * L_start:
             *   <eval cond into t>
             *   ifFalse t goto L_end
             *   <body>
             *   <update>
             *   goto L_start
             * L_end:
             */
            char *l_start = tac_new_label(list);
            char *l_end = tac_new_label(list);

            if (node->data.for_stmt.init) {
                char *r = gen_expr(node->data.for_stmt.init, list);
                free(r);
            }
            tac_emit(list, TAC_LABEL, l_start, NULL, NULL);
            if (node->data.for_stmt.condition) {
                char *cond = gen_expr(node->data.for_stmt.condition, list);
                tac_emit(list, TAC_IF_FALSE_GOTO, l_end, cond, NULL);
                free(cond);
            }
            gen_stmt(node->data.for_stmt.body, list);
            if (node->data.for_stmt.update) {
                char *r = gen_expr(node->data.for_stmt.update, list);
                free(r);
            }
            tac_emit(list, TAC_GOTO, l_start, NULL, NULL);
            tac_emit(list, TAC_LABEL, l_end, NULL, NULL);

            free(l_start); free(l_end);
            break;
        }

        case NODE_RETURN_STMT: {
            if (node->data.return_stmt.expr) {
                char *val = gen_expr(node->data.return_stmt.expr, list);
                tac_emit(list, TAC_RETURN, NULL, val, NULL);
                free(val);
            } else {
                tac_emit(list, TAC_RETURN, NULL, NULL, NULL);
            }
            break;
        }

        case NODE_PRINTF_STMT: {
            /* Parse format string for %d, %f, %c, %s specifiers */
            const char *fmt = node->data.printf_stmt.format_str;
            int arg_idx = 0;

            /* First, handle the case where there are no format args */
            if (node->data.printf_stmt.arg_count == 0) {
                char quoted[256];
                snprintf(quoted, sizeof(quoted), "\"%s\"", fmt);
                tac_emit(list, TAC_PRINT_STR, NULL, quoted, NULL);
                break;
            }

            /* Walk through format string, emit print for each segment */
            char buffer[256];
            int buf_idx = 0;

            for (int i = 0; fmt[i]; i++) {
                if (fmt[i] == '%' && fmt[i+1] && arg_idx < node->data.printf_stmt.arg_count) {
                    /* Flush any accumulated text */
                    if (buf_idx > 0) {
                        buffer[buf_idx] = '\0';
                        char quoted[256];
                        snprintf(quoted, sizeof(quoted), "\"%s\"", buffer);
                        tac_emit(list, TAC_PRINT_STR, NULL, quoted, NULL);
                        buf_idx = 0;
                    }
                    /* Emit print for the argument */
                    char *arg_val = gen_expr(node->data.printf_stmt.args[arg_idx], list);
                    char spec = fmt[i+1];
                    switch (spec) {
                        case 'd': tac_emit(list, TAC_PRINT_INT, NULL, arg_val, NULL); break;
                        case 'f': tac_emit(list, TAC_PRINT_FLOAT, NULL, arg_val, NULL); break;
                        case 'c': tac_emit(list, TAC_PRINT_CHAR, NULL, arg_val, NULL); break;
                        case 's': tac_emit(list, TAC_PRINT_STR, NULL, arg_val, NULL); break;
                        default:  tac_emit(list, TAC_PRINT_INT, NULL, arg_val, NULL); break;
                    }
                    free(arg_val);
                    arg_idx++;
                    i++; /* Skip format specifier character */
                } else if (fmt[i] == '\\' && fmt[i+1] == 'n') {
                    /* Handle \n */
                    if (buf_idx > 0) {
                        buffer[buf_idx] = '\0';
                        char quoted[256];
                        snprintf(quoted, sizeof(quoted), "\"%s\"", buffer);
                        tac_emit(list, TAC_PRINT_STR, NULL, quoted, NULL);
                        buf_idx = 0;
                    }
                    tac_emit(list, TAC_PRINT_STR, NULL, "\"\\n\"", NULL);
                    i++; /* Skip 'n' */
                } else {
                    buffer[buf_idx++] = fmt[i];
                }
            }
            /* Flush remaining text */
            if (buf_idx > 0) {
                buffer[buf_idx] = '\0';
                char quoted[256];
                snprintf(quoted, sizeof(quoted), "\"%s\"", buffer);
                tac_emit(list, TAC_PRINT_STR, NULL, quoted, NULL);
            }
            break;
        }

        default:
            break;
    }
}

/* ──────────────────────────────────────────────
 * Public: Generate TAC from AST root
 * ────────────────────────────────────────────── */
void tac_generate(ASTNode *root, TACList *list) {
    tac_init(list);
    gen_stmt(root, list);
}

/* ──────────────────────────────────────────────
 * TAC operation name for display
 * ────────────────────────────────────────────── */
static const char *tac_op_str(TACOp op) {
    switch (op) {
        case TAC_ADD:  return "+";
        case TAC_SUB:  return "-";
        case TAC_MUL:  return "*";
        case TAC_DIV:  return "/";
        case TAC_MOD:  return "%";
        case TAC_EQ:   return "==";
        case TAC_NEQ:  return "!=";
        case TAC_LT:   return "<";
        case TAC_GT:   return ">";
        case TAC_LTE:  return "<=";
        case TAC_GTE:  return ">=";
        case TAC_AND:  return "&&";
        case TAC_OR:   return "||";
        default: return "?";
    }
}

/* ──────────────────────────────────────────────
 * Phase 4 Output: Print TAC Instructions
 *
 * Format matches PDF example:
 *   1:  t1 = 3 * 2
 *   2:  t2 = 5 + t1
 *   3:  x = t2
 * ────────────────────────────────────────────── */
void print_tac(TACList *list) {
    printf("\n");
    printf("==========================================================\n");
    printf("   PHASE 4: INTERMEDIATE CODE (Three-Address Code)        \n");
    printf("==========================================================\n\n");

    printf("  TAC Instructions:\n");
    printf("  ---------------------------------------------\n");

    int line = 1;
    TACInstr *instr = list->head;
    while (instr) {
        printf("  %3d:  ", line++);

        switch (instr->op) {
            case TAC_ADD: case TAC_SUB: case TAC_MUL:
            case TAC_DIV: case TAC_MOD:
            case TAC_EQ:  case TAC_NEQ:
            case TAC_LT:  case TAC_GT:
            case TAC_LTE: case TAC_GTE:
            case TAC_AND: case TAC_OR:
                printf("%s = %s %s %s\n", instr->result, instr->arg1,
                       tac_op_str(instr->op), instr->arg2);
                break;

            case TAC_NEG:
                printf("%s = -%s\n", instr->result, instr->arg1);
                break;

            case TAC_NOT:
                printf("%s = !%s\n", instr->result, instr->arg1);
                break;

            case TAC_COPY:
                printf("%s = %s\n", instr->result, instr->arg1);
                break;

            case TAC_LABEL:
                printf("%s:\n", instr->result);
                break;

            case TAC_GOTO:
                printf("goto %s\n", instr->result);
                break;

            case TAC_IF_GOTO:
                printf("if %s goto %s\n", instr->arg1, instr->result);
                break;

            case TAC_IF_FALSE_GOTO:
                printf("ifFalse %s goto %s\n", instr->arg1, instr->result);
                break;

            case TAC_PARAM:
                printf("param %s\n", instr->arg1);
                break;

            case TAC_CALL:
                printf("%s = call %s, %s\n", instr->result, instr->arg1, instr->arg2);
                break;

            case TAC_RETURN:
                if (instr->arg1[0])
                    printf("return %s\n", instr->arg1);
                else
                    printf("return\n");
                break;

            case TAC_PRINT_INT:
                printf("print_int %s\n", instr->arg1);
                break;

            case TAC_PRINT_FLOAT:
                printf("print_float %s\n", instr->arg1);
                break;

            case TAC_PRINT_STR:
                printf("print_str %s\n", instr->arg1);
                break;

            case TAC_PRINT_CHAR:
                printf("print_char %s\n", instr->arg1);
                break;

            case TAC_FUNC_BEGIN:
                printf("func_begin %s\n", instr->arg1);
                break;

            case TAC_FUNC_END:
                printf("func_end %s\n", instr->arg1);
                break;
        }

        instr = instr->next;
    }

    printf("\n  Total TAC instructions: %d\n\n", list->instr_count);
}

/* ──────────────────────────────────────────────
 * Free TAC instruction list
 * ────────────────────────────────────────────── */
void tac_free(TACList *list) {
    TACInstr *curr = list->head;
    while (curr) {
        TACInstr *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = list->tail = NULL;
    list->instr_count = 0;
}
