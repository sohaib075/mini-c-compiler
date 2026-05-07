/*
 * ============================================================
 *   MINI-C COMPILER — Phase 5: Code Optimization
 *   File: optimizer.h
 *   Purpose: Optimizer interface for constant folding and
 *            dead code elimination on TAC.
 * ============================================================
 */

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "tac.h"

/* ──────────────────────────────────────────────
 * Optimization Statistics
 * ────────────────────────────────────────────── */
typedef struct {
    int constants_folded;
    int dead_code_removed;
    int copy_propagated;
    char descriptions[128][256]; /* Textual description of each optimization */
    int  desc_count;
} OptStats;

/* ──────────────────────────────────────────────
 * Public API
 * ────────────────────────────────────────────── */

/* Run all optimization passes on the TAC list */
void optimize_tac(TACList *list, OptStats *stats);

/* Phase 5 Output: Print before/after comparison */
void print_optimization_report(TACList *original, TACList *optimized, OptStats *stats);

/* Deep copy a TAC list (for before/after comparison) */
void tac_copy(TACList *dest, TACList *src);

#endif /* OPTIMIZER_H */
