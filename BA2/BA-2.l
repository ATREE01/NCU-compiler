%{
    int multiConnent = 0;
    int prevIdent = 0;
    int curLineIdent = 0;
    void printStr(char *str, int start, int end) {
        for (int i = start; i < end; i++) {
            printf("%c", str[i]);
        }
    }

    void printRBR(){
        // printf("%d, %d", curLineIdent, prevIdent);

        if(curLineIdent < prevIdent) {
            for(int i = 0; i < prevIdent - curLineIdent - 1; i++) {
                printf("\t");
            }
            printf("}\n");
            prevIdent--;

            while(curLineIdent < prevIdent) {
                for(int i = 0; i < prevIdent - 1; i++) {
                    printf("\t");
                }
                printf("}\n");
                prevIdent--;
            }

            for(int i = 0; i < prevIdent; i++){
                printf("\t");
            }
        }

    }

    void printEndRBR(){
        while(prevIdent != curLineIdent){
            for(int i = 0; i < prevIdent - 1; i++) 
                printf("\t");
            printf("}\n");
            prevIdent--;
        }
    }

%}
%option noyywrap 
%%

\t {ECHO; curLineIdent++;}
"# ".*\n {printRBR(); printf("//"); printStr(yytext, 1, yyleng - 1); printf("\n"); curLineIdent = 0;}
"\"\"\"" {printRBR(); if(multiConnent == 0) printf("/*"); else printf("*/"); multiConnent = !multiConnent;}

"if True:" {printRBR(); if(multiConnent == 0) {printf("if(true) {"); prevIdent++;} else ECHO;}
"if False:" {printRBR();  if(multiConnent == 0){printf("if(false) {"); prevIdent++;} else ECHO;}

"print(".*")" {printRBR();   if(multiConnent == 0){printf("cout << "); printStr(yytext, 6, yyleng - 1); printf(" << endl;");} else ECHO;}

^\n {printRBR(); ECHO; curLineIdent = 0;}
\n {ECHO; curLineIdent = 0;}

<<EOF>> {printEndRBR(); return 0;}

%%
// Main function to start the lexical analysis
int main(int argc, char *argv[]) {
    yylex();  // Start processing input
    return 0; // Exit program
}
