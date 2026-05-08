/*
 * ============================================================
 *   MINI-C COMPILER — Phase 3: Semantic Analysis
 *   File: symtab.h
 *   Purpose: Symbol table and semantic analysis interface.
 * ============================================================
 */

#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* ──────────────────────────────────────────────
 * Symbol Table Configuration
 * ────────────────────────────────────────────── */
#define SYMTAB_SIZE 1024
#define MAX_SCOPES    64

/* ──────────────────────────────────────────────
 * Symbol Entry
 * ────────────────────────────────────────────── */
typedef struct {
    char name[128];
    char type[32];
    char scope[128];
    int  line_declared;
    int  is_function;
    int  param_count;
    int  is_used;
} Symbol;

/* ──────────────────────────────────────────────
 * Symbol Table State
 * ────────────────────────────────────────────── */
typedef struct {
    Symbol symbols[SYMTAB_SIZE];
    int    count;

    /* Scope management */
    char   scope_stack[MAX_SCOPES][128];
    int    scope_depth;
    int    scope_start[MAX_SCOPES];

    /* Error tracking */
    char   errors[256][256];
    int    error_count;
} SymbolTable;

/* ──────────────────────────────────────────────
 * Public API
 * ────────────────────────────────────────────── */
void symtab_init(SymbolTable *st);
int  symtab_insert(SymbolTable *st, const char *name, const char *type, int line, int is_function, int param_count);
Symbol *symtab_lookup(SymbolTable *st, const char *name);
Symbol *symtab_lookup_current_scope(SymbolTable *st, const char *name);

/* Scope Control */
void symtab_enter_scope(SymbolTable *st, const char *scope_name);
void symtab_exit_scope(SymbolTable *st);
const char *symtab_current_scope(SymbolTable *st);

/* Semantic Analysis */
void semantic_analysis(ASTNode *root, SymbolTable *st);
void semantic_error(SymbolTable *st, int line, const char *fmt, ...);

/* Phase 3 Output: Print Table & Errors */
void print_symbol_table(SymbolTable *st);

#endif /* SYMTAB_H */
