l [a-zA-Z]
%{

%}

%%

{l}+ {
        proc();
}

%%

void proc()
{
        char a = yytext[0];
        a = tolower(a);
        if(a == 'a' || a == 'e' || a == 'i' || a == 'o' || a == 'u')
        {
                printf("%say", yytext);
        }
        else
        {
                char* s = yytext + 1;
                printf("%s%cay", s, yytext[0]);
        }
}


int main(int argc, char** argv)
{
        if (argc != 2)
        {
                printf("Provide filename too");
                exit(1);
        }
        FILE* in = fopen(argv[1], "r");
        FILE* out = fopen("output.txt", "w");

        yyin = in;
//      dup2(fileno(out), 1);
        yylex();
        return 0;
}
