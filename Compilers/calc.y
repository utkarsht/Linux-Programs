%{
	#include <stdio.h>
	int yylex(void);
	int sym[26];
%}

%token VAR NUM
%start program
%left '+' '-'
%left '*' '/'

%%

program:
	program statement '\n'
	|	
	;

statement:
	expr	 			{	printf("%d\n", $1);	}
	| VAR '=' expr		{	sym[$1] = $3;		}
	;

expr:
	NUM
	| VAR				{	$$ = sym[$1];	 	}
	| expr '+' expr 	{	$$ = $1 + $3;	 	}
	| expr '-' expr		{	$$ = $1 - $3;		}
	| expr '*' expr		{	$$ = $1 * $3;		}
	| expr '/' expr		{	$$ = $1 / $3;		}
	| '(' expr ')' 		{	$$ = $2;			}
	;

%%

yyerror(char* s)
{

}

int main()
{
	yyparse();
	return 0;
}
