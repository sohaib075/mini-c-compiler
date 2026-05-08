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
        fprintf(stderr, "Usage: %s <input.c> [target_phase]\n", argv[0]);
        fprintf(stderr, "  target_phase (optional):\n");
        fprintf(stderr, "    1: Lexical Analysis only\n");
        fprintf(stderr, "    2: Syntax Analysis (AST)\n");
        fprintf(stderr, "    3: Semantic Analysis\n");
        fprintf(stderr, "    4: TAC Generation\n");
        fprintf(stderr, "    5: Code Optimization\n");
        fprintf(stderr, "    6: Full Compilation (default)\n");
        return 1;
    }

    const char *input_file = argv[1];
    int target_phase = 6; // Default: full compilation

    if (argc >= 3) {
        target_phase = atoi(argv[2]);
        if (target_phase < 1 || target_phase > 6) {
            fprintf(stderr, "Error: Invalid target phase %d. Use 1-6.\n", target_phase);
            return 1;
        }
    }

    /* Open input file */
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", input_file);
        return 1;
    }
    yyin = fp;

    /* Print banner */
    print_banner();
    printf("  Compiling: %s (Target Phase: %d)\n", input_file, target_phase);
    print_separator();

    /* Display source code */
    print_source_file(input_file);

    /* Phase 1: Lexical Analysis */
    int parse_result = yyparse();
    fclose(fp);

    print_token_table();
    print_separator();

    if (lex_error_count > 0) {
        printf("  Compilation stopped: %d lexical error(s) found.\n", lex_error_count);
        return 1;
    }

    if (target_phase == 1) {
        printf("  Stopping after Phase 1 (Lexical Analysis).\n");
        return 0;
    }

    /* Phase 2: Syntax Analysis (AST) */
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

    if (target_phase == 2) {
        printf("  Stopping after Phase 2 (Syntax Analysis).\n");
        free_ast(ast_root);
        return 0;
    }

    /* Phase 3: Semantic Analysis */
    SymbolTable symtab;
    semantic_analysis(ast_root, &symtab);
    print_symbol_table(&symtab);
    print_separator();

    if (symtab.error_count > 0) {
        printf("  Compilation stopped: %d semantic error(s) found.\n", symtab.error_count);
        free_ast(ast_root);
        return 1;
    }

    if (target_phase == 3) {
        printf("  Stopping after Phase 3 (Semantic Analysis).\n");
        free_ast(ast_root);
        return 0;
    }

    /* Phase 4: Intermediate Code Generation (TAC) */
    TACList tac_list;
    tac_generate(ast_root, &tac_list);
    print_tac(&tac_list);
    print_separator();

    if (target_phase == 4) {
        printf("  Stopping after Phase 4 (TAC Generation).\n");
        free_ast(ast_root);
        tac_free(&tac_list);
        return 0;
    }

    /* Phase 5: Code Optimization */
    TACList original_tac;
    tac_copy(&original_tac, &tac_list);

    OptStats opt_stats;
    optimize_tac(&tac_list, &opt_stats);

    print_optimization_report(&original_tac, &tac_list, &opt_stats);
    print_separator();

    if (target_phase == 5) {
        printf("  Stopping after Phase 5 (Optimization).\n");
        free_ast(ast_root);
        tac_free(&tac_list);
        tac_free(&original_tac);
        return 0;
    }

    /* Phase 6: Target Code Generation (x86 Assembly) */
    CodeGen codegen;
    codegen_generate(&tac_list, &codegen);
    print_assembly(&codegen);
    write_assembly_file(&codegen, "output/output.asm");

    /* Phase 7: Program Execution (Interpreter) */
    interpret_tac(&tac_list);

    /* Summary */
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

