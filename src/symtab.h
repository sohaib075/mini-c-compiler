/*
 * ============================================================
 *   MINI-C COMPILER — Phase 3: Semantic Analysis
 *   File: symtab.h
 *   Purpose: Symbol table data structure and semantic
 *            analysis function declarations.
 * ============================================================
 */

#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h"

/* ──────────────────────────────────────────────
 * Symbol Table Configuration
 * ────────────────────────────────────────────── */
#define SYMTAB_SIZE   256    /* Max symbols in one scope  */
#define MAX_SCOPES     32    /* Max nesting depth         */

/* ──────────────────────────────────────────────
 * Symbol Entry — One per variable/function
 * ────────────────────────────────────────────── */
typedef struct {
    char name[128];          /* Variable/function name    */
    char type[32];           /* Data type: int, float, char, void */
    char scope[128];         /* Scope: "global", "main()", etc.   */
    int  line_declared;      /* Source line number        */
    int  is_function;        /* 1 if function, 0 if variable      */
    int  param_count;        /* Number of parameters (functions)   */
    int  is_used;            /* Whether this symbol is referenced  */
} Symbol;

/* ──────────────────────────────────────────────
 * Symbol Table — Stack-based scope management
 * ────────────────────────────────────────────── */
typedef struct {
    Symbol symbols[SYMTAB_SIZE];
    int    count;

    /* Scope stack for nested blocks */
    char   scope_stack[MAX_SCOPES][128];
    int    scope_depth;

    /* Symbol counts at each scope entry (for cleanup) */
    int    scope_start[MAX_SCOPES];

    /* Error tracking */
    int    error_count;
    char   errors[256][256];
} SymbolTable;

/* ──────────────────────────────────────────────
 * Public API
 * ────────────────────────────────────────────── */

/* Initialize symbol table */
void symtab_init(SymbolTable *st);

/* Scope management */
void symtab_enter_scope(SymbolTable *st, const char *scope_name);
void symtab_exit_scope(SymbolTable *st);
const char *symtab_current_scope(SymbolTable *st);

/* Symbol operations */
int  symtab_insert(SymbolTable *st, const char *name, const char *type,
                   int line, int is_function, int param_count);
Symbol *symtab_lookup(SymbolTable *st, const char *name);
Symbol *symtab_lookup_current_scope(SymbolTable *st, const char *name);

/* Phase 3: Run semantic analysis on AST */
void semantic_analysis(ASTNode *root, SymbolTable *st);

/* Phase 3 Output: Print symbol table and errors */
void print_symbol_table(SymbolTable *st);

/* Error reporting */
void semantic_error(SymbolTable *st, int line, const char *fmt, ...);

#endif /* SYMTAB_H */
