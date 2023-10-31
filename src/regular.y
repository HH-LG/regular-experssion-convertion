%{
#include<stdlib.h>
#include<ctype.h>
#include <locale.h>
#include <iostream>
#include <wchar.h>
#include "nfa.h"
#include "dfa.h"
#include "mini.h"

int regularPrase();
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);

%}

%union{
    wchar_t chval;
    struct nfa* exprval;
}
// 运算符类型
%token OR
%token CLOSURE 
%token L_BRAC R_BRAC
%token QUIT
%token<chval> UNIT

// 结合律与优先级
%left OR        // 或
%right CLOSURE   // 闭包

// 开始符号与非终结符
%type<exprval> Expr
%type<exprval> Unit
%type<exprval> UnitSeq
%start line
%%


line   :       Expr ';'                         {
                                                    //printNFA($1);         // 打印NFA
                                                    buildDFA(*$1);              // 构建DFA
                                                    //printDFA(dfa_generated);    // 打印DFA
                                                    simplifyDFA(dfa_generated);                 // 简化DFA
                                                    printDFA(dfa_simplified);   // 打印简化后的DFA
                                                    CurrentState = 0;
                                                    return 0;
                                                }
        |       QUIT                            { exit(0); }
        |       // 空串
        ;

Expr    :       Expr OR UnitSeq                    { $$ = orExprval($1,$3); free($1); free($3); }
        |       UnitSeq                        { $$ = $1; }
        ;

UnitSeq:        Unit                            { $$ = $1; }
        |       UnitSeq Unit                   { $$ = connectExprval($1,$2); free($1); free($2); }
        ;

Unit    :       UNIT                            { $$ = newExprval($1); }
        |       Unit CLOSURE                    { $$ = closureExprval($1); free($1); }
        |       L_BRAC Expr R_BRAC              { $$ = $2;}
        ;


%%

// programs section

int yylex()
{
    int t;
    while(1){
        t=getc(yyin);
        if (t==' '||t=='\t'||t=='\n')
        {
            // do noting
        }
        else if (t == '|')
        {
            return OR;
        }
        else if (t == '*')
        {
            return CLOSURE;
        }
        else if (t == '(')
        {
            return L_BRAC;
        }
        else if (t == ')')
        {
            return R_BRAC;
        }
        else if (t == '?')
        {
            return QUIT;
        }
        else if (t == ';')
        {
            return t;
        }
        else if (t == 'q')
        {
            t = getc(yyin);
            if (t != '\n')
            {
                ungetc(t, yyin);
                yylval.chval = 'q';
                return UNIT;
            }
            return QUIT;
        }
        else{
            yylval.chval = t;
            return UNIT;
        }
    }
}


int regularPrase(void)
{
    yyin=stdin;
    do{
        yyparse();
    }while(!feof(yyin));
    return 0;
}
void yyerror(const char* s){
    fprintf(stderr,"Parse error: %s\n",s);
    exit(1);
}