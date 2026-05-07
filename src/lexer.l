/*
 * ============================================================
 *   MINI-C COMPILER — Phase 1: Lexical Analysis
 *   File: lexer.l (Flex specification)
 *   Purpose: Reads .c source file character-by-character,
 *            identifies and classifies all lexical tokens.
 *   Output:  Token stream table with Token Value, Token Type,
 *            and Line Number.
 * ============================================================
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"

/* ── Token tracking for Phase 1 output ── */
typedef struct {
    char value[256];
    char type[32];
    int  line;
} TokenRecord;

#define MAX_TOKENS 4096
TokenRecord token_table[MAX_TOKENS];
int token_count = 0;

/* Record a token for Phase 1 display */
static void record_token(const char *value, const char *type, int line) {
    if (token_count < MAX_TOKENS) {
        strncpy(token_table[token_count].value, value, 255);
        token_table[token_count].value[255] = '\0';
        strncpy(token_table[token_count].type, type, 31);
        token_table[token_count].type[31] = '\0';
        token_table[token_count].line = line;
        token_count++;
    }
}

/* Track line numbers */
int yyline = 1;

/* Error count for lexical errors */
int lex_error_count = 0;
%}

/* ── Flex options ── */
%option noyywrap
%option nounput
%option noinput

/* ── Regular expression definitions ── */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+"."{DIGIT}+
WHITESPACE  [ \t\r]+

%%

 /* ════════════════════════════════════
  *  Preprocessor directives — Skip
  * ════════════════════════════════════ */
"#".*                   { /* Skip preprocessor directives like #include */ }

 /* ════════════════════════════════════
  *  Comments — Skip entirely
  * ════════════════════════════════════ */
"//".*                  { /* Single-line comment: skip */ }
"/*"([^*]|\*+[^*/])*\*+"/"  { 
    /* Multi-line comment: count newlines inside */
    for (int i = 0; yytext[i]; i++) {
        if (yytext[i] == '\n') yyline++;
    }
}

 /* ════════════════════════════════════
  *  Whitespace & Newlines
  * ════════════════════════════════════ */
{WHITESPACE}            { /* Skip whitespace */ }
"\n"                    { yyline++; }

 /* ════════════════════════════════════
  *  Keywords — Must appear before ID rule
  * ════════════════════════════════════ */
"int"       { record_token(yytext, "KEYWORD", yyline); return INT; }
"float"     { record_token(yytext, "KEYWORD", yyline); return FLOAT; }
"char"      { record_token(yytext, "KEYWORD", yyline); return CHAR; }
"void"      { record_token(yytext, "KEYWORD", yyline); return VOID; }
"if"        { record_token(yytext, "KEYWORD", yyline); return IF; }
"else"      { record_token(yytext, "KEYWORD", yyline); return ELSE; }
"while"     { record_token(yytext, "KEYWORD", yyline); return WHILE; }
"for"       { record_token(yytext, "KEYWORD", yyline); return FOR; }
"return"    { record_token(yytext, "KEYWORD", yyline); return RETURN; }
"printf"    { record_token(yytext, "KEYWORD", yyline); return PRINTF; }

 /* ════════════════════════════════════
  *  Relational & Logical Operators
  *  (Multi-char operators BEFORE single-char)
  * ════════════════════════════════════ */
"=="        { record_token(yytext, "OPERATOR", yyline); return EQ; }
"!="        { record_token(yytext, "OPERATOR", yyline); return NEQ; }
"<="        { record_token(yytext, "OPERATOR", yyline); return LTE; }
">="        { record_token(yytext, "OPERATOR", yyline); return GTE; }
"&&"        { record_token(yytext, "OPERATOR", yyline); return AND; }
"||"        { record_token(yytext, "OPERATOR", yyline); return OR; }

 /* ════════════════════════════════════
  *  Single-character Operators
  * ════════════════════════════════════ */
"+"         { record_token(yytext, "OPERATOR", yyline); return PLUS; }
"-"         { record_token(yytext, "OPERATOR", yyline); return MINUS; }
"*"         { record_token(yytext, "OPERATOR", yyline); return MULTIPLY; }
"/"         { record_token(yytext, "OPERATOR", yyline); return DIVIDE; }
"%"         { record_token(yytext, "OPERATOR", yyline); return MODULO; }
"="         { record_token(yytext, "OPERATOR", yyline); return ASSIGN; }
"<"         { record_token(yytext, "OPERATOR", yyline); return LT; }
">"         { record_token(yytext, "OPERATOR", yyline); return GT; }
"!"         { record_token(yytext, "OPERATOR", yyline); return NOT; }

 /* ════════════════════════════════════
  *  Punctuation
  * ════════════════════════════════════ */
";"         { record_token(yytext, "PUNCTUATION", yyline); return SEMICOLON; }
","         { record_token(yytext, "PUNCTUATION", yyline); return COMMA; }
"("         { record_token(yytext, "PUNCTUATION", yyline); return LPAREN; }
")"         { record_token(yytext, "PUNCTUATION", yyline); return RPAREN; }
"{"         { record_token(yytext, "PUNCTUATION", yyline); return LBRACE; }
"}"         { record_token(yytext, "PUNCTUATION", yyline); return RBRACE; }

 /* ════════════════════════════════════
  *  Literals
  * ════════════════════════════════════ */
{FLOAT}     { 
    record_token(yytext, "FLOAT_LITERAL", yyline);
    yylval.fval = atof(yytext);
    return FLOAT_LITERAL;
}
{INTEGER}   {
    record_token(yytext, "INTEGER_LITERAL", yyline);
    yylval.ival = atoi(yytext);
    return INTEGER_LITERAL;
}

 /* Character literal: 'x' */
"'"([^'\\]|\\.)"'"  {
    record_token(yytext, "CHAR_LITERAL", yyline);
    yylval.cval = yytext[1];
    return CHAR_LITERAL;
}

 /* String literal: "..." */
\"([^"\\]|\\.)*\"   {
    record_token(yytext, "STRING_LITERAL", yyline);
    /* Remove surrounding quotes */
    char *s = strdup(yytext + 1);
    s[strlen(s) - 1] = '\0';
    yylval.str = s;
    return STRING_LITERAL;
}

 /* ════════════════════════════════════
  *  Identifiers (after keywords!)
  * ════════════════════════════════════ */
{ID}        {
    record_token(yytext, "IDENTIFIER", yyline);
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}

 /* ════════════════════════════════════
  *  Error: Unrecognized character
  * ════════════════════════════════════ */
.           {
    fprintf(stderr, "Lexical Error at line %d: Unrecognized character '%s'\n", yyline, yytext);
    lex_error_count++;
}

%%

/* ──────────────────────────────────────────────
 * Phase 1 Output: Print Token Stream Table
 * Matches the exact format required by the PDF:
 *   Token Value | Token Type | Line No.
 * ────────────────────────────────────────────── */
void print_token_table(void) {
    printf("\n");
    printf("==========================================================\n");
    printf("           PHASE 1: LEXICAL ANALYSIS (Tokenization)       \n");
    printf("==========================================================\n\n");

    printf("  %-20s %-20s %s\n", "Token Value", "Token Type", "Line No.");
    printf("  %-20s %-20s %s\n", "-------------------", "-------------------", "--------");

    for (int i = 0; i < token_count; i++) {
        printf("  %-20s %-20s %d\n",
               token_table[i].value,
               token_table[i].type,
               token_table[i].line);
    }

    printf("\n  Total tokens: %d\n", token_count);
    if (lex_error_count > 0) {
        printf("  Lexical errors: %d\n", lex_error_count);
    } else {
        printf("  Lexical Analysis Passed -- No errors found.\n");
    }
    printf("\n");
}
