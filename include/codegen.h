/*
 * ============================================================
 *   MINI-C COMPILER — Phase 6: Target Code Generation
 *   File: codegen.h
 *   Purpose: x86 assembly code generator interface.
 * ============================================================
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "tac.h"

/* ──────────────────────────────────────────────
 * Code Generator Configuration
 * ────────────────────────────────────────────── */
#define MAX_VARS      128
#define MAX_ASM_LINES 2048

/* ──────────────────────────────────────────────
 * Variable → Memory mapping
 * ────────────────────────────────────────────── */
typedef struct {
    char name[64];
    int  is_temp;        /* Is this a temporary? */
} VarEntry;

/* ──────────────────────────────────────────────
 * Code Generator State
 * ────────────────────────────────────────────── */
typedef struct {
    VarEntry vars[MAX_VARS];
    int      var_count;

    char     asm_lines[MAX_ASM_LINES][256];
    int      asm_count;
} CodeGen;

/* ──────────────────────────────────────────────
 * Public API
 * ────────────────────────────────────────────── */

/* Generate x86 assembly from optimized TAC */
void codegen_generate(TACList *tac, CodeGen *cg);

/* Phase 6 Output: Print generated assembly */
void print_assembly(CodeGen *cg);

/* Write assembly to output file */
void write_assembly_file(CodeGen *cg, const char *filename);

#endif /* CODEGEN_H */
