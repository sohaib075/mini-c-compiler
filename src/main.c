/*
 * ============================================================
 *   MINI-C COMPILER — Main Driver
 *   File: main.c
 *   Purpose: Orchestrates all six compiler phases in order:
 *     Phase 1: Lexical Analysis   (lexer.l → tokens)
 *     Phase 2: Syntax Analysis    (parser.y → AST)
 *     Phase 3: Semantic Analysis  (symtab.c → validation)
 *     Phase 4: TAC Generation     (tac.c → intermediate code)
 *     Phase 5: Code Optimization  (optimizer.c → optimized TAC)
 *     Phase 6: Code Generation    (codegen.c → x86 assembly)
 *
 *   Usage: ./minicc <input.c>
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "symtab.h"
#include "tac.h"
#include "optimizer.h"
#include "codegen.h"
#include "interpreter.h"

/* ── External declarations from lexer/parser ── */
extern FILE *yyin;
extern int yyparse(void);
extern int yyline;
extern int lex_error_count;
extern int syntax_error_count;
extern ASTNode *ast_root;
extern void print_token_table(void);

/* ──────────────────────────────────────────────
 * Print a decorative header banner
 * ────────────────────────────────────────────── */
static void print_banner(void) {
    printf("\n");
    printf("************************************************************\n");
    printf("*                                                          *\n");
    printf("*              MINI-C COMPILER v1.0                        *\n");
    printf("*       FAST National University - CFD Campus              *\n");
    printf("*       Department of Computer Science                     *\n");
    printf("*       Compiler Construction Project                      *\n");
    printf("*                                                          *\n");
    printf("************************************************************\n");
    printf("\n");
}


/* ──────────────────────────────────────────────
 * Print a phase separator
 * ────────────────────────────────────────────── */
static void print_separator(void) {
    printf("\n----------------------------------------------------------\n");
}

/* ──────────────────────────────────────────────
 * Read and display the input source file
 * ────────────────────────────────────────────── */
static void print_source_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    printf("  Input Source File: %s\n", filename);
    printf("  ---------------------------------------------\n");

    char line[1024];
    int line_num = 1;
    while (fgets(line, sizeof(line), fp)) {
        printf("  %3d | %s", line_num++, line);
        if (line[strlen(line)-1] != '\n') printf("\n");
    }
    printf("  ---------------------------------------------\n\n");

    fclose(fp);
}

/* ──────────────────────────────────────────────
 * MAIN: Entry point — run all 6 phases
 * ────────────────────────────────────────────── */
int main(int argc, char *argv[]) {
    /* Validate command-line arguments */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.c>\n", argv[0]);
        fprintf(stderr, "  Compiles a Mini-C source file through all 6 phases.\n");
        return 1;
    }

    const char *input_file = argv[1];

    /* Open input file */
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", input_file);
        return 1;
    }
    yyin = fp;

    /* Print banner */
    print_banner();
    printf("  Compiling: %s\n", input_file);
    print_separator();

    /* Display source code */
    print_source_file(input_file);

    /* ════════════════════════════════════════════
     *  PHASE 1: LEXICAL ANALYSIS
     *
     *  The lexer (lexer.l / lex.yy.c) reads the
     *  source file character by character and
     *  produces a token stream. Tokens are recorded
     *  in token_table[] for display.
     *
     *  Note: yyparse() drives yylex() internally,
     *  so Phase 1 and 2 run together, but the
     *  token table is captured separately.
     * ════════════════════════════════════════════ */

    /* Parse (this drives the lexer too) */
    int parse_result = yyparse();
    fclose(fp);

    /* Print Phase 1 output: Token Stream Table */
    print_token_table();
    print_separator();

    /* Check for lexical errors */
    if (lex_error_count > 0) {
        printf("  Compilation stopped: %d lexical error(s) found.\n", lex_error_count);
        return 1;
    }

    /* ════════════════════════════════════════════
     *  PHASE 2: SYNTAX ANALYSIS
     *
     *  The parser (parser.y / parser.tab.c) checks
     *  that the token stream matches the Mini-C
     *  grammar. On success, it produces an AST.
     *  The AST is printed as an indented tree.
     * ════════════════════════════════════════════ */

    printf("\n");
    printf("==========================================================\n");
    printf("           PHASE 2: SYNTAX ANALYSIS (AST)                 \n");
    printf("==========================================================\n\n");

    if (parse_result != 0 || syntax_error_count > 0 || !ast_root) {
        printf("  Syntax errors found: %d\n", syntax_error_count);
        printf("  Compilation stopped due to syntax errors.\n\n");
        if (ast_root) free_ast(ast_root);
        return 1;
    }

    printf("  Abstract Syntax Tree:\n");
    printf("  ---------------------------------------------\n  ");
    print_ast(ast_root, 0);
    printf("\n  Syntax Analysis Passed -- AST constructed successfully.\n");
    print_separator();

    /* ════════════════════════════════════════════
     *  PHASE 3: SEMANTIC ANALYSIS
     *
     *  Traverses the AST and builds a symbol table.
     *  Checks for: undeclared variables, duplicate
     *  declarations, type mismatches, scope errors.
     * ════════════════════════════════════════════ */

    SymbolTable symtab;
    semantic_analysis(ast_root, &symtab);
    print_symbol_table(&symtab);
    print_separator();

    if (symtab.error_count > 0) {
        printf("  Compilation stopped: %d semantic error(s) found.\n", symtab.error_count);
        free_ast(ast_root);
        return 1;
    }

    /* ════════════════════════════════════════════
     *  PHASE 4: INTERMEDIATE CODE GENERATION
     *
     *  Walks the validated AST and produces
     *  Three-Address Code (TAC) using temporary
     *  variables (t0, t1, ...) and labels (L0, L1, ...).
     * ════════════════════════════════════════════ */

    TACList tac_list;
    tac_generate(ast_root, &tac_list);
    print_tac(&tac_list);
    print_separator();

    /* ════════════════════════════════════════════
     *  PHASE 5: CODE OPTIMIZATION
     *
     *  Creates a copy of the TAC for comparison,
     *  then applies:
     *    - Constant Folding
     *    - Copy Propagation
     *    - Dead Code Elimination
     *  Shows before/after comparison.
     * ════════════════════════════════════════════ */

    /* Save a copy of the original TAC for comparison */
    TACList original_tac;
    tac_copy(&original_tac, &tac_list);

    /* Run optimizer on the working copy */
    OptStats opt_stats;
    optimize_tac(&tac_list, &opt_stats);

    print_optimization_report(&original_tac, &tac_list, &opt_stats);
    print_separator();

    /* ════════════════════════════════════════════
     *  PHASE 6: TARGET CODE GENERATION
     *
     *  Translates the optimized TAC to x86-style
     *  assembly language with .data and .text sections.
     *  Output is printed and saved to a file.
     * ════════════════════════════════════════════ */

    CodeGen codegen;
    codegen_generate(&tac_list, &codegen);
    print_assembly(&codegen);

    /* Write assembly to output file */
    write_assembly_file(&codegen, "output/output.asm");

    /* ════════════════════════════════════════════
     *  PHASE 7: PROGRAM EXECUTION
     *
     *  Interprets the optimized TAC to execute
     *  the program and display actual output.
     * ════════════════════════════════════════════ */

    interpret_tac(&tac_list);

    /* ════════════════════════════════════════════
     *  COMPILATION COMPLETE
     * ════════════════════════════════════════════ */

    printf("************************************************************\n");
    printf("*           COMPILATION COMPLETED SUCCESSFULLY             *\n");
    printf("************************************************************\n");
    printf("\n");
    printf("  Summary:\n");
    printf("    Phase 1 (Lexical Analysis):    PASSED\n");
    printf("    Phase 2 (Syntax Analysis):     PASSED\n");
    printf("    Phase 3 (Semantic Analysis):   PASSED\n");
    printf("    Phase 4 (TAC Generation):      %d instructions\n", original_tac.instr_count);
    printf("    Phase 5 (Optimization):        %d optimizations applied\n",
           opt_stats.constants_folded + opt_stats.dead_code_removed + opt_stats.copy_propagated);
    printf("    Phase 6 (Code Generation):     %d assembly lines\n", codegen.asm_count);
    printf("    Phase 7 (Execution):           COMPLETED\n");
    printf("\n");
    printf("  Output file: output/output.asm\n");
    printf("\n");

    /* Cleanup */
    free_ast(ast_root);
    tac_free(&tac_list);
    tac_free(&original_tac);

    return 0;
}
