/*
 * ============================================================
 *   MINI-C COMPILER — Phase 5: Code Optimization
 *   File: optimizer.c
 *   Purpose: Applies optimization passes to the intermediate
 *            TAC representation:
 *              1. Constant Folding — evaluate constant
 *                 expressions at compile time
 *              2. Dead Code Elimination — remove unused
 *                 temporary assignments
 *              3. Copy Propagation — replace copies with
 *                 the original value
 *   Output:  Before/after TAC comparison showing applied
 *            optimizations.
 * ============================================================
 */

#include "optimizer.h"
#include <ctype.h>

/* ──────────────────────────────────────────────
 * Helper: Check if a string is a numeric constant
 * ────────────────────────────────────────────── */
static int is_numeric(const char *s) {
    if (!s || !s[0]) return 0;
    int i = 0;
    if (s[0] == '-') i = 1;
    int has_dot = 0;
    for (; s[i]; i++) {
        if (s[i] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        } else if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
    }
    return (i > 0 && (i > 1 || s[0] != '-'));
}

/* ──────────────────────────────────────────────
 * Helper: Check if a name is a temporary (t0, t1, ...)
 * ────────────────────────────────────────────── */
static int is_temp(const char *s) {
    return (s && s[0] == 't' && isdigit((unsigned char)s[1]));
}

/* ──────────────────────────────────────────────
 * Helper: Count how many times a temp is used as
 * an argument in the TAC list
 * ────────────────────────────────────────────── */
static int count_uses(TACList *list, const char *temp) {
    int count = 0;
    TACInstr *instr = list->head;
    while (instr) {
        if (strcmp(instr->arg1, temp) == 0) count++;
        if (strcmp(instr->arg2, temp) == 0) count++;
        /* Also count uses in ifFalse/if goto conditions */
        if ((instr->op == TAC_IF_FALSE_GOTO || instr->op == TAC_IF_GOTO) &&
            strcmp(instr->arg1, temp) == 0) {
            /* Already counted above */
        }
        instr = instr->next;
    }
    return count;
}

/* ──────────────────────────────────────────────
 * Pass 1: Constant Folding
 *
 * Finds instructions like:
 *   t1 = 5 + 3
 * and replaces them with:
 *   t1 = 8
 *
 * Handles +, -, *, /, %, ==, !=, <, >, <=, >=
 * ────────────────────────────────────────────── */
static void constant_folding(TACList *list, OptStats *stats) {
    TACInstr *instr = list->head;
    while (instr) {
        if (is_numeric(instr->arg1) && is_numeric(instr->arg2)) {
            double a = atof(instr->arg1);
            double b = atof(instr->arg2);
            double result = 0;
            int can_fold = 1;
            char original[128];

            snprintf(original, sizeof(original), "%s = %s %s %s",
                     instr->result, instr->arg1,
                     instr->op == TAC_ADD ? "+" :
                     instr->op == TAC_SUB ? "-" :
                     instr->op == TAC_MUL ? "*" :
                     instr->op == TAC_DIV ? "/" :
                     instr->op == TAC_MOD ? "%" : "?",
                     instr->arg2);

            switch (instr->op) {
                case TAC_ADD: result = a + b; break;
                case TAC_SUB: result = a - b; break;
                case TAC_MUL: result = a * b; break;
                case TAC_DIV:
                    if (b != 0) result = a / b;
                    else can_fold = 0;
                    break;
                case TAC_MOD:
                    if (b != 0) result = (int)a % (int)b;
                    else can_fold = 0;
                    break;
                case TAC_EQ:  result = (a == b) ? 1 : 0; break;
                case TAC_NEQ: result = (a != b) ? 1 : 0; break;
                case TAC_LT:  result = (a < b) ? 1 : 0; break;
                case TAC_GT:  result = (a > b) ? 1 : 0; break;
                case TAC_LTE: result = (a <= b) ? 1 : 0; break;
                case TAC_GTE: result = (a >= b) ? 1 : 0; break;
                default: can_fold = 0; break;
            }

            if (can_fold) {
                /* Replace with copy of constant result */
                if (result == (int)result) {
                    sprintf(instr->arg1, "%d", (int)result);
                } else {
                    sprintf(instr->arg1, "%.2f", result);
                }
                instr->arg2[0] = '\0';
                instr->op = TAC_COPY;

                stats->constants_folded++;
                snprintf(stats->descriptions[stats->desc_count++], 256,
                         "Constant Folding: %s -> %s = %s",
                         original, instr->result, instr->arg1);
            }
        }
        instr = instr->next;
    }
}

/* ──────────────────────────────────────────────
 * Pass 2: Copy Propagation
 *
 * Finds instructions like:
 *   t1 = 5
 *   x = t1
 * and replaces uses of t1 with 5 directly.
 * ────────────────────────────────────────────── */
static void copy_propagation(TACList *list, OptStats *stats) {
    TACInstr *instr = list->head;
    while (instr) {
        /* Look for: temp = constant (where temp is a temporary) */
        if (instr->op == TAC_COPY && is_temp(instr->result) &&
            (is_numeric(instr->arg1) || !is_temp(instr->arg1))) {

            /* Replace all uses of this temp with the value */
            TACInstr *scan = instr->next;
            while (scan) {
                if (strcmp(scan->arg1, instr->result) == 0) {
                    strncpy(scan->arg1, instr->arg1, 63);
                }
                if (strcmp(scan->arg2, instr->result) == 0) {
                    strncpy(scan->arg2, instr->arg1, 63);
                }
                scan = scan->next;
            }
            stats->copy_propagated++;
        }
        instr = instr->next;
    }
}

/* ──────────────────────────────────────────────
 * Pass 3: Dead Code Elimination
 *
 * Removes temporary assignments where the
 * temporary is never used (or has been
 * propagated away).
 * ────────────────────────────────────────────── */
static void dead_code_elimination(TACList *list, OptStats *stats) {
    TACInstr *prev = NULL;
    TACInstr *instr = list->head;

    while (instr) {
        TACInstr *next = instr->next;

        /* Check if this assigns to a temp that is never used */
        if (instr->op == TAC_COPY && is_temp(instr->result)) {
            int uses = count_uses(list, instr->result);
            if (uses == 0) {
                /* Remove this instruction */
                if (prev) {
                    prev->next = next;
                } else {
                    list->head = next;
                }
                if (instr == list->tail) {
                    list->tail = prev;
                }

                stats->dead_code_removed++;
                snprintf(stats->descriptions[stats->desc_count++], 256,
                         "Dead Code Elimination: Removed '%s = %s' (unused temporary)",
                         instr->result, instr->arg1);

                free(instr);
                list->instr_count--;
                instr = next;
                continue;
            }
        }

        prev = instr;
        instr = next;
    }
}

/* ──────────────────────────────────────────────
 * Public: Run all optimization passes
 *
 * Order matters:
 *   1. Constant Folding (simplify expressions)
 *   2. Copy Propagation (eliminate temporaries)
 *   3. Dead Code Elimination (remove unused)
 *
 * We iterate multiple times until no more
 * changes are made (fixed-point iteration).
 * ────────────────────────────────────────────── */
void optimize_tac(TACList *list, OptStats *stats) {
    stats->constants_folded = 0;
    stats->dead_code_removed = 0;
    stats->copy_propagated = 0;
    stats->desc_count = 0;

    int changed = 1;
    int iterations = 0;

    while (changed && iterations < 10) {
        int old_folded = stats->constants_folded;
        int old_removed = stats->dead_code_removed;
        int old_propagated = stats->copy_propagated;

        constant_folding(list, stats);
        copy_propagation(list, stats);
        dead_code_elimination(list, stats);

        changed = (stats->constants_folded != old_folded ||
                   stats->dead_code_removed != old_removed ||
                   stats->copy_propagated != old_propagated);
        iterations++;
    }
}

/* ──────────────────────────────────────────────
 * Deep copy a TAC list
 * ────────────────────────────────────────────── */
void tac_copy(TACList *dest, TACList *src) {
    tac_init(dest);
    TACInstr *instr = src->head;
    while (instr) {
        tac_emit(dest, instr->op, instr->result, instr->arg1, instr->arg2);
        instr = instr->next;
    }
}

/* ──────────────────────────────────────────────
 * Phase 5 Output: Show optimization report
 * ────────────────────────────────────────────── */
void print_optimization_report(TACList *original, TACList *optimized, OptStats *stats) {
    printf("\n");
    printf("==========================================================\n");
    printf("              PHASE 5: CODE OPTIMIZATION                   \n");
    printf("==========================================================\n\n");

    printf("  Original TAC (%d instructions):\n", original->instr_count);
    printf("  ---------------------------------------------\n");
    {
        int line = 1;
        TACInstr *instr = original->head;
        while (instr) {
            printf("    %3d:  ", line++);
            switch (instr->op) {
                case TAC_ADD: case TAC_SUB: case TAC_MUL:
                case TAC_DIV: case TAC_MOD:
                case TAC_EQ: case TAC_NEQ: case TAC_LT:
                case TAC_GT: case TAC_LTE: case TAC_GTE:
                case TAC_AND: case TAC_OR: {
                    const char *op_s =
                        instr->op == TAC_ADD ? "+" : instr->op == TAC_SUB ? "-" :
                        instr->op == TAC_MUL ? "*" : instr->op == TAC_DIV ? "/" :
                        instr->op == TAC_MOD ? "%" : instr->op == TAC_EQ ? "==" :
                        instr->op == TAC_NEQ ? "!=" : instr->op == TAC_LT ? "<" :
                        instr->op == TAC_GT ? ">" : instr->op == TAC_LTE ? "<=" :
                        instr->op == TAC_GTE ? ">=" : instr->op == TAC_AND ? "&&" : "||";
                    printf("%s = %s %s %s\n", instr->result, instr->arg1, op_s, instr->arg2);
                    break;
                }
                case TAC_COPY:
                    printf("%s = %s\n", instr->result, instr->arg1);
                    break;
                case TAC_LABEL:
                    printf("%s:\n", instr->result);
                    break;
                case TAC_GOTO:
                    printf("goto %s\n", instr->result);
                    break;
                case TAC_IF_FALSE_GOTO:
                    printf("ifFalse %s goto %s\n", instr->arg1, instr->result);
                    break;
                case TAC_FUNC_BEGIN:
                    printf("func_begin %s\n", instr->arg1);
                    break;
                case TAC_FUNC_END:
                    printf("func_end %s\n", instr->arg1);
                    break;
                case TAC_RETURN:
                    printf("return %s\n", instr->arg1);
                    break;
                case TAC_PRINT_INT:
                    printf("print_int %s\n", instr->arg1);
                    break;
                case TAC_PRINT_STR:
                    printf("print_str %s\n", instr->arg1);
                    break;
                default:
                    printf("...\n");
                    break;
            }
            instr = instr->next;
        }
    }

    printf("\n  Optimized TAC (%d instructions):\n", optimized->instr_count);
    printf("  ---------------------------------------------\n");
    print_tac(optimized);

    printf("  Optimizations Applied:\n");
    if (stats->constants_folded > 0)
        printf("    Constant Folding:       %d expression(s) folded\n", stats->constants_folded);
    if (stats->copy_propagated > 0)
        printf("    Copy Propagation:       %d copies propagated\n", stats->copy_propagated);
    if (stats->dead_code_removed > 0)
        printf("    Dead Code Elimination:  %d instruction(s) removed\n", stats->dead_code_removed);

    if (stats->desc_count > 0) {
        printf("\n  Details:\n");
        for (int i = 0; i < stats->desc_count && i < 20; i++) {
            printf("    - %s\n", stats->descriptions[i]);
        }
    }

    int total = stats->constants_folded + stats->dead_code_removed + stats->copy_propagated;
    if (total == 0) {
        printf("    No optimizations applicable.\n");
    }
    printf("\n");
}
