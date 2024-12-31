%{
    void print(char* type, char *str) {
        for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] == '\n') continue;
            printf("%c", str[i]);
        }
        printf(" %s\n", type);
    }
%}
%option noyywrap 
%%

^[0-9]+[\+\-\*\/]([\+\-\*\/]|[0-9])* {print("OPERATION", yytext); }
^[0-9]+\.[0-9]+  {print("FLOAT", yytext);}
^[0-9]+  {print("INTEGER", yytext); }
^(true|false)  {print("BOOLEAN", yytext);}
^\"[^\"\n]*\"  {print("STRING", yytext);}
^\'[^\'\n]*\'  {print("STRING", yytext);}
^.* {print("INVALID", yytext);}
\n {}
%%
// Main function to start the lexical analysis
int main(int argc, char *argv[]) {
    yylex();  // Start processing input
    return 0; // Exit program
}

// .*\n {if(!flag) printf("%.*s INVALID\n", yyleng-1, yytext); }