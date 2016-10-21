%{
	#include <stdlib.h>
	#include "y.tab.h"
%}

%%

[a-z] 		{	 yylval = *yytext - 'a';		return VAR;		}
[0-9]+		{	 yylval = atoi(yytext);			return NUM;		}
[-+=()*/\n] {	 return *yytext;	}
[ \t\n]		{}

%%
