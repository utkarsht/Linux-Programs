d [0-9]
ws [ \n\t\r]

%{
	#include <strings.h>
	int cnt = 0;
%}

%%
[+-]?[0-9]+("."[0-9]+)? {
	printf("Number\n");
}

[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,} {
	printf("Email \n");
}

ws+ {
	
}

%%

int main() 
{
	yylex();
	return 0;	
}
