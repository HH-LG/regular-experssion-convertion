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
    struct expr* exprval;
}
// 运算符类型
%token OR
%token CLOSURE 
%token L_BRAC R_BRAC
%token QUIT
%token<chval> UNIT

// 结合律与优先级
%left OR        // 或
%left CONNECT    //  连接
%right CLOSURE   // 闭包

// 开始符号与非终结符
%type<exprval> expr
%type<exprval> unit
%type<exprval> unit_seq
%start lines
%%


lines   :       lines expr ';'                  {
                                                    // printExprval($2); 
                                                    buildDFA(*$2);
                                                    printDFA(dfa_generated);
                                                    simplify();
                                                    printDFA(dfa_simplified);
                                                    CurrentState = 0;
                                                }
        |       lines QUIT                      { exit(0); }
        |       // 空串
        ;

expr    :       expr OR expr                    { $$ = orExprval($1,$3); free($1); free($3); }
        |       unit_seq                        { $$ = $1; }
        ;

unit_seq:       unit                            { $$ = $1; }
        |       unit_seq unit                   { $$ = connectExprval($1,$2); free($1); free($2); }
        ;

unit    :       UNIT                            { $$ = newExprval($1); }
        |       unit CLOSURE                    { $$ = closureExprval($1); free($1); }
        |       L_BRAC expr R_BRAC              { $$ = $2;}
        ;


%%

// programs section

int yylex()
{
    int t;
    while(1){
        t=getchar();
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