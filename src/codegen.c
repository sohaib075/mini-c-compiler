/*
 * ============================================================
 *   MINI-C COMPILER — Phase 6: Target Code Generation
 *   File: codegen.c
 *   Purpose: Translates optimized TAC into x86-style assembly
 *            language. Maps variables to memory, temporaries
 *            to registers, and generates .data/.text sections.
 *   Output:  x86 pseudo-assembly printed to console and
 *            written to output file.
 * ============================================================
 *
 *   Register Allocation Strategy (simple):
 *     - EAX: primary accumulator for arithmetic
 *     - EBX: secondary register for operands
 *     - ECX: loop counters / third operand
 *     - EDX: division remainder / fourth operand
 *     - Variables stored in .data section as dd (32-bit)
 *     - Temporaries mapped to stack or registers
 * ============================================================
 */

#include "codegen.h"
#include <ctype.h>
#include <stdarg.h>

/* ──────────────────────────────────────────────
 * Helper: Add a line of assembly
 * ────────────────────────────────────────────── */
static void emit_asm(CodeGen *cg, const char *fmt, ...) {
    if (cg->asm_count >= MAX_ASM_LINES) return;
    va_list args;
    va_start(args, fmt);
    vsnprintf(cg->asm_lines[cg->asm_count], 256, fmt, args);
    va_end(args);
    cg->asm_count++;
}

/* ──────────────────────────────────────────────
 * Helper: Check if string is a numeric constant
 * ────────────────────────────────────────────── */
static int cg_is_numeric(const char *s) {
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
 * Helper: Check if name is a temporary
 * ────────────────────────────────────────────── */
static int cg_is_temp(const char *s) {
    return (s && s[0] == 't' && isdigit((unsigned char)s[1]));
}

/* ──────────────────────────────────────────────
 * Helper: Register a variable if not already known
 * ────────────────────────────────────────────── */
static void register_var(CodeGen *cg, const char *name) {
    if (!name || name[0] == '\0') return;
    if (cg_is_numeric(name)) return;
    if (name[0] == '"') return;  /* String literal */
    if (name[0] == '\'') return; /* Char literal */

    for (int i = 0; i < cg->var_count; i++) {
        if (strcmp(cg->vars[i].name, name) == 0) return;
    }

    if (cg->var_count < MAX_VARS) {
        strncpy(cg->vars[cg->var_count].name, name, 63);
        cg->vars[cg->var_count].is_temp = cg_is_temp(name);
        cg->var_count++;
    }
}

/* ──────────────────────────────────────────────
 * Helper: Load a value into EAX
 * Value can be a constant or a variable name
 * ────────────────────────────────────────────── */
static void load_to_eax(CodeGen *cg, const char *val) {
    if (cg_is_numeric(val)) {
        emit_asm(cg, "    mov eax, %s", val);
    } else {
        register_var(cg, val);
        emit_asm(cg, "    mov eax, [%s]", val);
    }
}

/* ──────────────────────────────────────────────
 * Helper: Load a value into EBX
 * ────────────────────────────────────────────── */
static void load_to_ebx(CodeGen *cg, const char *val) {
    if (cg_is_numeric(val)) {
        emit_asm(cg, "    mov ebx, %s", val);
    } else {
        register_var(cg, val);
        emit_asm(cg, "    mov ebx, [%s]", val);
    }
}

/* ──────────────────────────────────────────────
 * Helper: Store EAX into a variable
 * ────────────────────────────────────────────── */
static void store_eax(CodeGen *cg, const char *dest) {
    register_var(cg, dest);
    emit_asm(cg, "    mov [%s], eax", dest);
}

/* ──────────────────────────────────────────────
 * Main Code Generation: TAC → x86 Assembly
 *
 * Processes each TAC instruction and emits
 * the corresponding x86 assembly instructions.
 * ────────────────────────────────────────────── */
void codegen_generate(TACList *tac, CodeGen *cg) {
    cg->var_count = 0;
    cg->asm_count = 0;

    /* -- First pass: collect all variable names -- */
    TACInstr *scan = tac->head;
    while (scan) {
        if (scan->result[0] && scan->op != TAC_LABEL &&
            scan->op != TAC_GOTO && scan->op != TAC_IF_GOTO &&
            scan->op != TAC_IF_FALSE_GOTO &&
            scan->op != TAC_FUNC_BEGIN && scan->op != TAC_FUNC_END &&
            scan->op != TAC_CALL && scan->op != TAC_RETURN &&
            scan->op != TAC_PARAM &&
            scan->op != TAC_PRINT_INT && scan->op != TAC_PRINT_FLOAT &&
            scan->op != TAC_PRINT_STR && scan->op != TAC_PRINT_CHAR) {
            register_var(cg, scan->result);
        }
        if (scan->arg1[0] && !cg_is_numeric(scan->arg1) &&
            scan->arg1[0] != '"' && scan->arg1[0] != '\'' &&
            scan->op != TAC_FUNC_BEGIN && scan->op != TAC_FUNC_END &&
            scan->op != TAC_CALL) {
            register_var(cg, scan->arg1);
        }
        if (scan->arg2[0] && !cg_is_numeric(scan->arg2) &&
            scan->arg2[0] != '"' && scan->arg2[0] != '\'' &&
            scan->op != TAC_CALL) {
            register_var(cg, scan->arg2);
        }
        scan = scan->next;
    }

    /* ── Emit .data section ── */
    emit_asm(cg, "section .data");
    for (int i = 0; i < cg->var_count; i++) {
        emit_asm(cg, "    %s dd 0", cg->vars[i].name);
    }
    /* String constants for printf */
    emit_asm(cg, "    fmt_int db \"%%d\", 0");
    emit_asm(cg, "    fmt_float db \"%%f\", 0");
    emit_asm(cg, "    fmt_char db \"%%c\", 0");
    emit_asm(cg, "    fmt_str db \"%%s\", 0");
    emit_asm(cg, "    newline db 10, 0");
    emit_asm(cg, "");

    /* ── Emit .text section ── */
    emit_asm(cg, "section .text");
    emit_asm(cg, "    global _main");
    emit_asm(cg, "    extern _printf");
    emit_asm(cg, "");

    /* ── Generate instructions ── */
    TACInstr *instr = tac->head;
    while (instr) {
        /* Add comment showing original TAC */
        switch (instr->op) {
            case TAC_FUNC_BEGIN:
                emit_asm(cg, "");
                emit_asm(cg, "_%s:", instr->arg1);
                emit_asm(cg, "    push ebp");
                emit_asm(cg, "    mov ebp, esp");
                break;

            case TAC_FUNC_END:
                /* Always emit epilogue - needed for void functions without return */
                emit_asm(cg, "    mov esp, ebp");
                emit_asm(cg, "    pop ebp");
                emit_asm(cg, "    ret");
                emit_asm(cg, "");
                break;

            case TAC_COPY:
                /* result = arg1 */
                load_to_eax(cg, instr->arg1);
                store_eax(cg, instr->result);
                break;

            case TAC_ADD:
                /* result = arg1 + arg2 */
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    add eax, ebx");
                store_eax(cg, instr->result);
                break;

            case TAC_SUB:
                /* result = arg1 - arg2 */
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    sub eax, ebx");
                store_eax(cg, instr->result);
                break;

            case TAC_MUL:
                /* result = arg1 * arg2 */
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    imul eax, ebx");
                store_eax(cg, instr->result);
                break;

            case TAC_DIV:
                /* result = arg1 / arg2 */
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    cdq");
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    idiv ebx");
                store_eax(cg, instr->result);
                break;

            case TAC_MOD:
                /* result = arg1 % arg2 (remainder in EDX) */
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    cdq");
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    idiv ebx");
                emit_asm(cg, "    mov [%s], edx", instr->result);
                register_var(cg, instr->result);
                break;

            case TAC_NEG:
                /* result = -arg1 */
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    neg eax");
                store_eax(cg, instr->result);
                break;

            case TAC_NOT:
                /* result = !arg1 (logical not) */
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    cmp eax, 0");
                emit_asm(cg, "    sete al");
                emit_asm(cg, "    movzx eax, al");
                store_eax(cg, instr->result);
                break;

            case TAC_EQ:
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    cmp eax, ebx");
                emit_asm(cg, "    sete al");
                emit_asm(cg, "    movzx eax, al");
                store_eax(cg, instr->result);
                break;

            case TAC_NEQ:
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    cmp eax, ebx");
                emit_asm(cg, "    setne al");
                emit_asm(cg, "    movzx eax, al");
                store_eax(cg, instr->result);
                break;

            case TAC_LT:
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    cmp eax, ebx");
                emit_asm(cg, "    setl al");
                emit_asm(cg, "    movzx eax, al");
                store_eax(cg, instr->result);
                break;

            case TAC_GT:
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    cmp eax, ebx");
                emit_asm(cg, "    setg al");
                emit_asm(cg, "    movzx eax, al");
                store_eax(cg, instr->result);
                break;

            case TAC_LTE:
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    cmp eax, ebx");
                emit_asm(cg, "    setle al");
                emit_asm(cg, "    movzx eax, al");
                store_eax(cg, instr->result);
                break;

            case TAC_GTE:
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    cmp eax, ebx");
                emit_asm(cg, "    setge al");
                emit_asm(cg, "    movzx eax, al");
                store_eax(cg, instr->result);
                break;

            case TAC_AND:
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    cmp eax, 0");
                emit_asm(cg, "    setne al");
                emit_asm(cg, "    movzx eax, al");
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    cmp ebx, 0");
                emit_asm(cg, "    setne bl");
                emit_asm(cg, "    movzx ebx, bl");
                emit_asm(cg, "    and eax, ebx");
                store_eax(cg, instr->result);
                break;

            case TAC_OR:
                load_to_eax(cg, instr->arg1);
                load_to_ebx(cg, instr->arg2);
                emit_asm(cg, "    or eax, ebx");
                emit_asm(cg, "    cmp eax, 0");
                emit_asm(cg, "    setne al");
                emit_asm(cg, "    movzx eax, al");
                store_eax(cg, instr->result);
                break;

            case TAC_LABEL:
                emit_asm(cg, "%s:", instr->result);
                break;

            case TAC_GOTO:
                emit_asm(cg, "    jmp %s", instr->result);
                break;

            case TAC_IF_GOTO:
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    cmp eax, 0");
                emit_asm(cg, "    jne %s", instr->result);
                break;

            case TAC_IF_FALSE_GOTO:
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    cmp eax, 0");
                emit_asm(cg, "    je %s", instr->result);
                break;

            case TAC_PARAM:
                /* Push parameter for function call */
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    push eax");
                break;

            case TAC_CALL:
                emit_asm(cg, "    call _%s", instr->arg1);
                if (instr->arg2[0]) {
                    int arg_count = atoi(instr->arg2);
                    if (arg_count > 0) {
                        emit_asm(cg, "    add esp, %d", arg_count * 4);
                    }
                }
                if (instr->result[0]) {
                    store_eax(cg, instr->result);
                }
                break;

            case TAC_RETURN:
                if (instr->arg1[0]) {
                    load_to_eax(cg, instr->arg1);
                }
                emit_asm(cg, "    mov esp, ebp");
                emit_asm(cg, "    pop ebp");
                emit_asm(cg, "    ret");
                break;

            case TAC_PRINT_INT:
                /* Call printf with %d format */
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    push eax");
                emit_asm(cg, "    push fmt_int");
                emit_asm(cg, "    call _printf");
                emit_asm(cg, "    add esp, 8");
                break;

            case TAC_PRINT_FLOAT:
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    push eax");
                emit_asm(cg, "    push fmt_float");
                emit_asm(cg, "    call _printf");
                emit_asm(cg, "    add esp, 8");
                break;

            case TAC_PRINT_CHAR:
                load_to_eax(cg, instr->arg1);
                emit_asm(cg, "    push eax");
                emit_asm(cg, "    push fmt_char");
                emit_asm(cg, "    call _printf");
                emit_asm(cg, "    add esp, 8");
                break;

            case TAC_PRINT_STR:
                /* Check if it's a newline */
                if (strcmp(instr->arg1, "\"\\n\"") == 0) {
                    emit_asm(cg, "    push newline");
                    emit_asm(cg, "    call _printf");
                    emit_asm(cg, "    add esp, 4");
                } else {
                    /* Push string literal directly */
                    emit_asm(cg, "    push %s", instr->arg1);
                    emit_asm(cg, "    call _printf");
                    emit_asm(cg, "    add esp, 4");
                }
                break;

            default:
                break;
        }

        instr = instr->next;
    }
}

/* ──────────────────────────────────────────────
 * Phase 6 Output: Print x86 Assembly
 *
 * Matches PDF format with .data and .text sections
 * ────────────────────────────────────────────── */
void print_assembly(CodeGen *cg) {
    printf("\n");
    printf("==========================================================\n");
    printf("       PHASE 6: TARGET CODE GENERATION (x86 Assembly)     \n");
    printf("==========================================================\n\n");

    for (int i = 0; i < cg->asm_count; i++) {
        printf("  %s\n", cg->asm_lines[i]);
    }
    printf("\n");
}

/* ──────────────────────────────────────────────
 * Write assembly to output file
 * ────────────────────────────────────────────── */
void write_assembly_file(CodeGen *cg, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open output file '%s'\n", filename);
        return;
    }

    fprintf(fp, "; ============================================\n");
    fprintf(fp, "; Mini-C Compiler Output — x86 Assembly\n");
    fprintf(fp, "; Generated automatically from Mini-C source\n");
    fprintf(fp, "; ============================================\n\n");

    for (int i = 0; i < cg->asm_count; i++) {
        fprintf(fp, "%s\n", cg->asm_lines[i]);
    }

    fclose(fp);
    printf("  Assembly written to: %s\n\n", filename);
}
