d [0-9]
l [a-zA-Z]
ws [ \n\t\r]

%{
        int kwcount = 0;
        int idcount = 0;
        int opcount = 0;
        int pcount = 0;
        int numcount = 0;
        int errcount = 0;
%}

%%
int|float|for|while|double|char|do {
        printf("keyword : %s\n", yytext);
        kwcount++;
}

{l}({l}|{d})* {
        printf("Identifier : %s\n", yytext);
        idcount++;
}
"+"|"-"|"*"|"="|"%"|"=="|"<="|"<"|"++"|"--" {
        printf ("Operator : %s\n", yytext);
        opcount++;
}
[,|;|)|(|{|}] {
        printf("Punctuation : %s\n", yytext);
        pcount++;
}
({d})*"."?({d})+ {
        printf("Constant : %s\n", yytext);
        numcount++;
}
"\""({l}|{d}|{ws})*"\"" {
        printf("String literal : %s\n", yytext);
}
{d}({l}{d})+ {
        printf("Error : %s\n", yytext);
        errcount++;
}
({ws})+ {
        //      do nothing
}
%%

int main()
{
        yylex();
        printf("Statistics :\n Keyword : %d\nIdentifier : %d\nOperator : %d\nPunctuation : %d\nConstant : %d\nError : %d\n");
        return 0;
}
