ws [ \t\r]

%{
        #include <stdlib.h>
        int space = 0;
%}

%%
{ws}+ {
        printf(" ");
}
%%

int main(int argc, char** argv)
{
        if(argc != 2)
        {
                printf("Provide name of input file");
                exit(1);
        }
        FILE* in = fopen(argv[1], "r");
        FILE* out = fopen("output.txt", "w");

        yyin = in;
//      yyout = out;
        dup2(fileno(out), 1);

        yylex();
        return 0;
}
