/*
 * ============================================================
 *   MINI-C COMPILER — Phase 2: Syntax Analysis (Parsing)
 *   File: parser.y (Bison grammar specification)
 *   Purpose: Parses token stream against Mini-C grammar,
 *            constructs an Abstract Syntax Tree (AST).
 *   Output:  Textual AST representation.
 * ============================================================
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* ── External declarations ── */
extern int yylex(void);
extern int yyline;
extern FILE *yyin;

/* ── The root of the AST ── */
ASTNode *ast_root = NULL;

/* ── Error handling ── */
int syntax_error_count = 0;

void yyerror(const char *msg) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yyline, msg);
    syntax_error_count++;
}

/* ── Helper: allocate param/arg arrays ── */
static ASTNode **alloc_node_array(int capacity) {
    return (ASTNode **)calloc(capacity, sizeof(ASTNode *));
}
%}

/* ──────────────────────────────────────────────
 * Semantic value types (YYSTYPE union)
 * Each token/rule carries one of these values
 * ────────────────────────────────────────────── */
%union {
    int    ival;        /* Integer literal value   */
    double fval;        /* Float literal value     */
    char   cval;        /* Char literal value      */
    char  *str;         /* String / identifier     */
    struct ASTNode *node;      /* AST node pointer */
    struct {
        struct ASTNode **nodes;
        int count;
    } node_list;        /* List of AST nodes       */
}

/* ──────────────────────────────────────────────
 * Token declarations with types
 * ────────────────────────────────────────────── */
%token INT FLOAT CHAR VOID
%token <str>  IDENTIFIER STRING_LITERAL
%token <ival> INTEGER_LITERAL
%token <fval> FLOAT_LITERAL
%token <cval> CHAR_LITERAL
%token IF ELSE WHILE FOR RETURN PRINTF
%token PLUS MINUS MULTIPLY DIVIDE MODULO
%token ASSIGN EQ NEQ LT GT LTE GTE
%token AND OR NOT
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE

/* ──────────────────────────────────────────────
 * Non-terminal types
 * ────────────────────────────────────────────── */
%type <node> program declaration var_declaration fun_declaration
%type <node> compound_stmt statement expression_stmt
%type <node> selection_stmt iteration_stmt return_stmt printf_stmt
%type <node> expression assignment_expr or_expr and_expr
%type <node> equality_expr relational_expr additive_expr
%type <node> multiplicative_expr unary_expr primary_expr
%type <node> param
%type <str>  type_specifier
%type <node_list> declaration_list statement_list param_list arg_list

/* ──────────────────────────────────────────────
 * Operator precedence (low → high)
 * Resolves dangling-else ambiguity
 * ────────────────────────────────────────────── */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left OR
%left AND
%left EQ NEQ
%left LT GT LTE GTE
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right NOT UMINUS

/* ── Start symbol ── */
%start program

%%

/* ════════════════════════════════════════════════
 *  GRAMMAR RULES — Mini-C Language
 * ════════════════════════════════════════════════ */

/* ── Program: list of global declarations ── */
program
    : declaration_list {
        ast_root = create_program_node(1);
        for (int i = 0; i < $1.count; i++) {
            program_add_declaration(ast_root, $1.nodes[i]);
        }
        free($1.nodes);
    }
    ;

declaration_list
    : declaration {
        $$.nodes = alloc_node_array(MAX_CHILDREN);
        $$.count = 0;
        $$.nodes[$$.count++] = $1;
    }
    | declaration_list declaration {
        $$ = $1;
        $$.nodes[$$.count++] = $2;
    }
    ;

/* ── Declaration: variable or function ── */
declaration
    : var_declaration   { $$ = $1; }
    | fun_declaration   { $$ = $1; }
    ;

/* ── Variable Declaration ── */
var_declaration
    : type_specifier IDENTIFIER SEMICOLON {
        $$ = create_var_decl_node($1, $2, NULL, yyline);
        free($1); free($2);
    }
    | type_specifier IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = create_var_decl_node($1, $2, $4, yyline);
        free($1); free($2);
    }
    ;

/* ── Type Specifier ── */
type_specifier
    : INT    { $$ = strdup("int"); }
    | FLOAT  { $$ = strdup("float"); }
    | CHAR   { $$ = strdup("char"); }
    | VOID   { $$ = strdup("void"); }
    ;

/* ── Function Declaration ── */
fun_declaration
    : type_specifier IDENTIFIER LPAREN param_list RPAREN compound_stmt {
        $$ = create_fun_decl_node($1, $2, $4.nodes, $4.count, $6, yyline);
        free($1); free($2);
    }
    | type_specifier IDENTIFIER LPAREN RPAREN compound_stmt {
        $$ = create_fun_decl_node($1, $2, NULL, 0, $5, yyline);
        free($1); free($2);
    }
    ;

/* ── Parameter List ── */
param_list
    : param {
        $$.nodes = alloc_node_array(MAX_PARAMS);
        $$.count = 0;
        $$.nodes[$$.count++] = $1;
    }
    | param_list COMMA param {
        $$ = $1;
        $$.nodes[$$.count++] = $3;
    }
    ;

param
    : type_specifier IDENTIFIER {
        $$ = create_param_node($1, $2, yyline);
        free($1); free($2);
    }
    ;

/* ── Compound Statement (Block) ── */
compound_stmt
    : LBRACE statement_list RBRACE {
        $$ = create_compound_node(yyline);
        for (int i = 0; i < $2.count; i++) {
            compound_add_item($$, $2.nodes[i]);
        }
        free($2.nodes);
    }
    | LBRACE RBRACE {
        $$ = create_compound_node(yyline);
    }
    ;

/* ── Statement List ── */
statement_list
    : statement {
        $$.nodes = alloc_node_array(MAX_CHILDREN);
        $$.count = 0;
        if ($1) $$.nodes[$$.count++] = $1;
    }
    | statement_list statement {
        $$ = $1;
        if ($2) $$.nodes[$$.count++] = $2;
    }
    ;

/* ── Statements ── */
statement
    : var_declaration     { $$ = $1; }
    | expression_stmt     { $$ = $1; }
    | compound_stmt       { $$ = $1; }
    | selection_stmt      { $$ = $1; }
    | iteration_stmt      { $$ = $1; }
    | return_stmt         { $$ = $1; }
    | printf_stmt         { $$ = $1; }
    ;

/* ── Expression Statement ── */
expression_stmt
    : expression SEMICOLON {
        $$ = create_expr_stmt_node($1, yyline);
    }
    | SEMICOLON {
        $$ = create_expr_stmt_node(NULL, yyline);
    }
    ;

/* ── If / If-Else ── */
selection_stmt
    : IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE {
        $$ = create_if_node($3, $5, NULL, yyline);
    }
    | IF LPAREN expression RPAREN statement ELSE statement {
        $$ = create_if_node($3, $5, $7, yyline);
    }
    ;

/* ── While / For Loops ── */
iteration_stmt
    : WHILE LPAREN expression RPAREN statement {
        $$ = create_while_node($3, $5, yyline);
    }
    | FOR LPAREN expression_stmt expression_stmt expression RPAREN statement {
        ASTNode *init = $3->data.expr_stmt.expr;
        ASTNode *cond = $4->data.expr_stmt.expr;
        $$ = create_for_node(init, cond, $5, $7, yyline);
    }
    ;

/* ── Return Statement ── */
return_stmt
    : RETURN expression SEMICOLON {
        $$ = create_return_node($2, yyline);
    }
    | RETURN SEMICOLON {
        $$ = create_return_node(NULL, yyline);
    }
    ;

/* ── Printf Statement ── */
printf_stmt
    : PRINTF LPAREN STRING_LITERAL RPAREN SEMICOLON {
        $$ = create_printf_node($3, NULL, 0, yyline);
        free($3);
    }
    | PRINTF LPAREN STRING_LITERAL COMMA arg_list RPAREN SEMICOLON {
        $$ = create_printf_node($3, $5.nodes, $5.count, yyline);
        free($3);
    }
    ;

/* ════════════════════════════════════════════════
 *  EXPRESSIONS — Operator Precedence Climbing
 * ════════════════════════════════════════════════ */

expression
    : assignment_expr     { $$ = $1; }
    | or_expr             { $$ = $1; }
    ;

/* ── Assignment: x = expr ── */
assignment_expr
    : IDENTIFIER ASSIGN expression {
        $$ = create_assign_node($1, $3, yyline);
        free($1);
    }
    ;

/* ── Logical OR: || ── */
or_expr
    : and_expr            { $$ = $1; }
    | or_expr OR and_expr {
        $$ = create_binary_node("||", $1, $3, yyline);
    }
    ;

/* ── Logical AND: && ── */
and_expr
    : equality_expr       { $$ = $1; }
    | and_expr AND equality_expr {
        $$ = create_binary_node("&&", $1, $3, yyline);
    }
    ;

/* ── Equality: == != ── */
equality_expr
    : relational_expr     { $$ = $1; }
    | equality_expr EQ relational_expr {
        $$ = create_binary_node("==", $1, $3, yyline);
    }
    | equality_expr NEQ relational_expr {
        $$ = create_binary_node("!=", $1, $3, yyline);
    }
    ;

/* ── Relational: < > <= >= ── */
relational_expr
    : additive_expr       { $$ = $1; }
    | relational_expr LT additive_expr {
        $$ = create_binary_node("<", $1, $3, yyline);
    }
    | relational_expr GT additive_expr {
        $$ = create_binary_node(">", $1, $3, yyline);
    }
    | relational_expr LTE additive_expr {
        $$ = create_binary_node("<=", $1, $3, yyline);
    }
    | relational_expr GTE additive_expr {
        $$ = create_binary_node(">=", $1, $3, yyline);
    }
    ;

/* ── Additive: + - ── */
additive_expr
    : multiplicative_expr { $$ = $1; }
    | additive_expr PLUS multiplicative_expr {
        $$ = create_binary_node("+", $1, $3, yyline);
    }
    | additive_expr MINUS multiplicative_expr {
        $$ = create_binary_node("-", $1, $3, yyline);
    }
    ;

/* ── Multiplicative: * / % ── */
multiplicative_expr
    : unary_expr          { $$ = $1; }
    | multiplicative_expr MULTIPLY unary_expr {
        $$ = create_binary_node("*", $1, $3, yyline);
    }
    | multiplicative_expr DIVIDE unary_expr {
        $$ = create_binary_node("/", $1, $3, yyline);
    }
    | multiplicative_expr MODULO unary_expr {
        $$ = create_binary_node("%", $1, $3, yyline);
    }
    ;

/* ── Unary: - ! ── */
unary_expr
    : primary_expr        { $$ = $1; }
    | MINUS unary_expr %prec UMINUS {
        $$ = create_unary_node("-", $2, yyline);
    }
    | NOT unary_expr {
        $$ = create_unary_node("!", $2, yyline);
    }
    ;

/* ── Primary Expressions ── */
primary_expr
    : INTEGER_LITERAL {
        $$ = create_int_literal_node($1, yyline);
    }
    | FLOAT_LITERAL {
        $$ = create_float_literal_node($1, yyline);
    }
    | CHAR_LITERAL {
        $$ = create_char_literal_node($1, yyline);
    }
    | STRING_LITERAL {
        $$ = create_string_literal_node($1, yyline);
        free($1);
    }
    | IDENTIFIER {
        $$ = create_identifier_node($1, yyline);
        free($1);
    }
    | IDENTIFIER LPAREN arg_list RPAREN {
        $$ = create_func_call_node($1, $3.nodes, $3.count, yyline);
        free($1);
    }
    | IDENTIFIER LPAREN RPAREN {
        $$ = create_func_call_node($1, NULL, 0, yyline);
        free($1);
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

/* ── Argument List (for function calls) ── */
arg_list
    : expression {
        $$.nodes = alloc_node_array(MAX_ARGS);
        $$.count = 0;
        $$.nodes[$$.count++] = $1;
    }
    | arg_list COMMA expression {
        $$ = $1;
        $$.nodes[$$.count++] = $3;
    }
    ;

%%
