/*
 * ============================================================
 *   MINI-C COMPILER — Phase 7: Program Execution
 *   File: interpreter.h
 *   Purpose: TAC Interpreter that walks optimized TAC,
 *            evaluates expressions, executes control flow,
 *            handles function calls, and prints output.
 * ============================================================
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "tac.h"

/* Run the TAC program and produce actual output */
void interpret_tac(TACList *list);

#endif /* INTERPRETER_H */
