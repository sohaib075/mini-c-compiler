/*
 * ============================================================
 *   MINI-C COMPILER — Phase 7: Program Execution
 *   File: interpreter.h
 *   Purpose: TAC interpreter interface for final program run.
 * ============================================================
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "tac.h"

/* ──────────────────────────────────────────────
 * Public API
 * ────────────────────────────────────────────── */

/* Execute the program by interpreting its TAC instructions */
void interpret_tac(TACList *list);

#endif /* INTERPRETER_H */
