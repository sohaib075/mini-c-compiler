/*
 * ============================================================
 *   MINI-C COMPILER — Phase 7: Program Execution
 *   File: interpreter.c
 *   Purpose: Interprets the optimized Three-Address Code (TAC)
 *            to execute the program and display actual output.
 *            Walks through TAC instructions sequentially,
 *            evaluates expressions, handles control flow
 *            (if/else, while, for), function calls, and
 *            prints printf() results to screen.
 * ============================================================
 */

#include "interpreter.h"
#include <ctype.h>
#include <string.h>
#include <math.h>

/* ---- Variable Storage ---- */
typedef struct {
    char name[64];
    int  value;
} InterpVar;

#define MAX_INTERP_VARS 512
static InterpVar interp_vars[MAX_INTERP_VARS];
static int interp_var_count = 0;

/* ---- Call Stack for function calls ---- */
#define MAX_CALL_DEPTH 64
static TACInstr *call_return_addr[MAX_CALL_DEPTH];
static int call_depth = 0;

/* ---- Parameter passing ---- */
#define MAX_PARAMS 32
static int param_stack[MAX_PARAMS];
static int param_count = 0;

/* ---- Helper: Check if string is numeric ---- */
static int interp_is_numeric(const char *s) {
    if (!s || !s[0]) return 0;
    int i = 0;
    if (s[0] == '-') i = 1;
    for (; s[i]; i++) {
        if (s[i] == '.' || !isdigit((unsigned char)s[i]))
            if (s[i] != '.') return 0;
    }
    return (i > 0 && (i > 1 || s[0] != '-'));
}

/* ---- Get variable value (or parse constant) ---- */
static int get_val(const char *name) {
    if (!name || !name[0]) return 0;

    /* Numeric constant */
    if (interp_is_numeric(name)) return atoi(name);

    /* Lookup variable */
    for (int i = 0; i < interp_var_count; i++) {
        if (strcmp(interp_vars[i].name, name) == 0)
            return interp_vars[i].value;
    }
    return 0;
}

/* ---- Set variable value ---- */
static void set_val(const char *name, int value) {
    if (!name || !name[0]) return;

    /* Update existing */
    for (int i = 0; i < interp_var_count; i++) {
        if (strcmp(interp_vars[i].name, name) == 0) {
            interp_vars[i].value = value;
            return;
        }
    }

    /* Insert new */
    if (interp_var_count < MAX_INTERP_VARS) {
        strncpy(interp_vars[interp_var_count].name, name, 63);
        interp_vars[interp_var_count].name[63] = '\0';
        interp_vars[interp_var_count].value = value;
        interp_var_count++;
    }
}

/* ---- Find a label instruction ---- */
static TACInstr *find_label(TACList *list, const char *label) {
    TACInstr *instr = list->head;
    while (instr) {
        if (instr->op == TAC_LABEL && strcmp(instr->result, label) == 0)
            return instr;
        instr = instr->next;
    }
    return NULL;
}

/* ---- Find func_begin for a function ---- */
static TACInstr *find_func(TACList *list, const char *name) {
    TACInstr *instr = list->head;
    while (instr) {
        if (instr->op == TAC_FUNC_BEGIN && strcmp(instr->arg1, name) == 0)
            return instr;
        instr = instr->next;
    }
    return NULL;
}

/* ---- Process escape sequences in format strings ---- */
static void print_format_str(const char *s) {
    /* Remove surrounding quotes if present */
    int start = 0, end = (int)strlen(s);
    if (s[0] == '"') start = 1;
    if (end > 0 && s[end - 1] == '"') end--;

    for (int i = start; i < end; i++) {
        if (s[i] == '\\' && i + 1 < end) {
            switch (s[i + 1]) {
                case 'n': printf("\n"); i++; break;
                case 't': printf("\t"); i++; break;
                case '\\': printf("\\"); i++; break;
                case '"': printf("\""); i++; break;
                default: printf("%c", s[i]); break;
            }
        } else {
            printf("%c", s[i]);
        }
    }
}

/* ============================================================
 * Main Interpreter: Execute TAC Instructions
 * ============================================================ */
void interpret_tac(TACList *list) {
    interp_var_count = 0;
    call_depth = 0;
    param_count = 0;

    printf("\n");
    printf("==========================================================\n");
    printf("       PHASE 7: PROGRAM EXECUTION (TAC Interpreter)       \n");
    printf("==========================================================\n\n");
    printf("  Program Output:\n");
    printf("  ---------------------------------------------\n");
    printf("  ");

    /* Start execution from main */
    TACInstr *instr = find_func(list, "main");
    if (!instr) {
        printf("Error: No 'main' function found.\n");
        return;
    }

    int has_output = 0;
    int max_steps = 100000;  /* Safety limit to prevent infinite loops */
    int steps = 0;

    while (instr && steps < max_steps) {
        steps++;

        switch (instr->op) {
            case TAC_FUNC_BEGIN:
                /* Function entry - load params as local vars */
                break;

            case TAC_FUNC_END:
                /* If we're in a called function, return to caller */
                if (call_depth > 0) {
                    call_depth--;
                    instr = call_return_addr[call_depth];
                    continue;
                }
                /* main() ended */
                goto execution_done;

            case TAC_COPY:
                set_val(instr->result, get_val(instr->arg1));
                break;

            case TAC_ADD:
                set_val(instr->result, get_val(instr->arg1) + get_val(instr->arg2));
                break;

            case TAC_SUB:
                set_val(instr->result, get_val(instr->arg1) - get_val(instr->arg2));
                break;

            case TAC_MUL:
                set_val(instr->result, get_val(instr->arg1) * get_val(instr->arg2));
                break;

            case TAC_DIV: {
                int divisor = get_val(instr->arg2);
                if (divisor != 0)
                    set_val(instr->result, get_val(instr->arg1) / divisor);
                else
                    set_val(instr->result, 0);
                break;
            }

            case TAC_MOD: {
                int divisor = get_val(instr->arg2);
                if (divisor != 0)
                    set_val(instr->result, get_val(instr->arg1) % divisor);
                else
                    set_val(instr->result, 0);
                break;
            }

            case TAC_NEG:
                set_val(instr->result, -get_val(instr->arg1));
                break;

            case TAC_NOT:
                set_val(instr->result, !get_val(instr->arg1));
                break;

            case TAC_EQ:
                set_val(instr->result, get_val(instr->arg1) == get_val(instr->arg2));
                break;

            case TAC_NEQ:
                set_val(instr->result, get_val(instr->arg1) != get_val(instr->arg2));
                break;

            case TAC_LT:
                set_val(instr->result, get_val(instr->arg1) < get_val(instr->arg2));
                break;

            case TAC_GT:
                set_val(instr->result, get_val(instr->arg1) > get_val(instr->arg2));
                break;

            case TAC_LTE:
                set_val(instr->result, get_val(instr->arg1) <= get_val(instr->arg2));
                break;

            case TAC_GTE:
                set_val(instr->result, get_val(instr->arg1) >= get_val(instr->arg2));
                break;

            case TAC_AND:
                set_val(instr->result, get_val(instr->arg1) && get_val(instr->arg2));
                break;

            case TAC_OR:
                set_val(instr->result, get_val(instr->arg1) || get_val(instr->arg2));
                break;

            case TAC_LABEL:
                /* Labels are just markers, no action */
                break;

            case TAC_GOTO: {
                TACInstr *target = find_label(list, instr->result);
                if (target) {
                    instr = target;
                    continue;
                }
                break;
            }

            case TAC_IF_GOTO: {
                if (get_val(instr->arg1) != 0) {
                    TACInstr *target = find_label(list, instr->result);
                    if (target) {
                        instr = target;
                        continue;
                    }
                }
                break;
            }

            case TAC_IF_FALSE_GOTO: {
                if (get_val(instr->arg1) == 0) {
                    TACInstr *target = find_label(list, instr->result);
                    if (target) {
                        instr = target;
                        continue;
                    }
                }
                break;
            }

            case TAC_PARAM:
                if (param_count < MAX_PARAMS) {
                    param_stack[param_count++] = get_val(instr->arg1);
                }
                break;

            case TAC_CALL: {
                TACInstr *func = find_func(list, instr->arg1);
                if (func) {
                    if (call_depth < MAX_CALL_DEPTH) {
                        call_return_addr[call_depth++] = instr->next;
                    }
                    param_count = 0;  /* Reset params after call setup */
                    instr = func;
                    continue;
                }
                break;
            }

            case TAC_RETURN:
                if (instr->arg1[0]) {
                    set_val("__retval__", get_val(instr->arg1));
                }
                /* Return to caller */
                if (call_depth > 0) {
                    call_depth--;
                    TACInstr *ret_addr = call_return_addr[call_depth];
                    /* Store return value in the call's result register */
                    if (ret_addr) {
                        /* Find the CALL instruction (it's the one before ret_addr) */
                        TACInstr *scan = list->head;
                        while (scan && scan->next != ret_addr) scan = scan->next;
                        if (scan && scan->op == TAC_CALL && scan->result[0]) {
                            set_val(scan->result, get_val("__retval__"));
                        }
                    }
                    instr = ret_addr;
                    continue;
                }
                /* Return from main */
                goto execution_done;

            case TAC_PRINT_INT:
                printf("%d", get_val(instr->arg1));
                has_output = 1;
                break;

            case TAC_PRINT_FLOAT:
                printf("%.2f", (double)get_val(instr->arg1));
                has_output = 1;
                break;

            case TAC_PRINT_CHAR:
                printf("%c", (char)get_val(instr->arg1));
                has_output = 1;
                break;

            case TAC_PRINT_STR:
                print_format_str(instr->arg1);
                has_output = 1;
                break;

            default:
                break;
        }

        instr = instr->next;
    }

execution_done:
    if (!has_output) {
        printf("(no output)");
    }
    printf("\n");
    printf("  ---------------------------------------------\n");

    if (steps >= max_steps) {
        printf("  Warning: Execution stopped after %d steps (possible infinite loop)\n", max_steps);
    }

    printf("\n");
}
