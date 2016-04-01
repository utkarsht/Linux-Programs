l [a-zA-Z]
d [0-9]
%{
        int len = 0;
        char res[100];
        int flag = 0;
        int prev = -1;
        int cur;
        FILE* fo;
        FILE* fd;
%}

%%

({d})+ {
        if (prev == -1)
                prev = atoi(yytext);
        else
        {
                cur = atoi(yytext);
                if(cur - prev > 0)
                {
                        fprintf(fo, "+");
                        fprintf(fd, "+%d", cur - prev);
                }
                else if(cur - prev < 0)
                {
                        fprintf(fo, "-");
                        fprintf(fd, "%d", cur - prev);
                }
                else
                {
                        fprintf(fo, "0");
                        fprintf(fd, "0");
                }
                printf("%d %d\n", prev, cur);
                prev = cur;
        }
}

[+]+[0]{0,3}[-]+/n {
        if(len < strlen(yytext))
        {
                strcpy(res, yytext);
                len = strlen(yytext);
        }
        printf("found : %s\n", yytext);
}

n|[\n] {
        if(flag)
        {
                printf("Longest feature is %s of length %d\n", res, len);
                len = 0;
        }
        else
        {
                prev = -1;
                fprintf(fo, "n");
        }
}

<<EOF>> {
        return;
}

%%

int yywrap() {
        return 1;
}

int main() {

        FILE* in = fopen("image.txt", "r");
        char buf[100];

        yyin = in;
/*
        while (fgets(buf, 100, in)) {
                printf("%s", buf);
        }
*/

        fo = fopen("ddash.txt", "w");
        fd = fopen("d.txt", "w");
        yylex();
        fclose(fo);
        fclose(fd);

        flag = 1;
        fo = fopen("ddash.txt", "r");
        yyin = fo;
        yylex();

        return 0;
}