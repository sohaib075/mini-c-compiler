/*
 * ============================================================
 *   MINI-C COMPILER — Phase 4: Intermediate Code Generation
 *   File: tac.h
 *   Purpose: Three-Address Code (TAC) instruction definitions
 *            and generator interface.
 * ============================================================
 */

#ifndef TAC_H
#define TAC_H

#include "ast.h"
#include "symtab.h"

/* ──────────────────────────────────────────────
 * TAC Operation Types
 * ────────────────────────────────────────────── */
typedef enum {
    TAC_ADD,            /* result = arg1 + arg2          */
    TAC_SUB,            /* result = arg1 - arg2          */
    TAC_MUL,            /* result = arg1 * arg2          */
    TAC_DIV,            /* result = arg1 / arg2          */
    TAC_MOD,            /* result = arg1 % arg2          */
    TAC_NEG,            /* result = -arg1                */
    TAC_NOT,            /* result = !arg1                */
    TAC_COPY,           /* result = arg1                 */
    TAC_EQ,             /* result = (arg1 == arg2)       */
    TAC_NEQ,            /* result = (arg1 != arg2)       */
    TAC_LT,             /* result = (arg1 < arg2)        */
    TAC_GT,             /* result = (arg1 > arg2)        */
    TAC_LTE,            /* result = (arg1 <= arg2)       */
    TAC_GTE,            /* result = (arg1 >= arg2)       */
    TAC_AND,            /* result = (arg1 && arg2)       */
    TAC_OR,             /* result = (arg1 || arg2)       */
    TAC_LABEL,          /* result:                       */
    TAC_GOTO,           /* goto result                   */
    TAC_IF_GOTO,        /* if arg1 goto result           */
    TAC_IF_FALSE_GOTO,  /* ifFalse arg1 goto result      */
    TAC_PARAM,          /* param arg1                    */
    TAC_CALL,           /* result = call arg1, arg2(cnt) */
    TAC_RETURN,         /* return arg1                   */
    TAC_PRINT_INT,      /* print_int arg1                */
    TAC_PRINT_FLOAT,    /* print_float arg1              */
    TAC_PRINT_STR,      /* print_str arg1                */
    TAC_PRINT_CHAR,     /* print_char arg1               */
    TAC_FUNC_BEGIN,     /* func_begin arg1               */
    TAC_FUNC_END,       /* func_end arg1                 */
} TACOp;

/* ──────────────────────────────────────────────
 * TAC Instruction — Linked list node
 * ────────────────────────────────────────────── */
typedef struct TACInstr {
    TACOp  op;
    char   result[64];
    char   arg1[64];
    char   arg2[64];
    int    index;           /* Instruction number */
    struct TACInstr *next;
} TACInstr;

/* ──────────────────────────────────────────────
 * TAC Generator State
 * ────────────────────────────────────────────── */
typedef struct {
    TACInstr *head;         /* First instruction  */
    TACInstr *tail;         /* Last instruction   */
    int       instr_count;  /* Total instructions */
    int       temp_count;   /* Next temp variable: t0, t1, ... */
    int       label_count;  /* Next label: L0, L1, ...         */
} TACList;

/* ──────────────────────────────────────────────
 * Public API
 * ────────────────────────────────────────────── */
void tac_init(TACList *list);
TACInstr *tac_emit(TACList *list, TACOp op, const char *result,
                    const char *arg1, const char *arg2);
char *tac_new_temp(TACList *list);
char *tac_new_label(TACList *list);

/* Generate TAC from validated AST */
void tac_generate(ASTNode *root, TACList *list);

/* Phase 4 Output: Print TAC instructions */
void print_tac(TACList *list);

/* Free TAC list */
void tac_free(TACList *list);

#endif /* TAC_H */
