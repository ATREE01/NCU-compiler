%{
    #include "B.tab.h"
    int charCnt = 0;
%}
%option noyywrap
%%

\n      { /* do nothing */ }
[ \t]+  { charCnt += yyleng; }
"+"|"-" { charCnt += yyleng; yylval.oper = charCnt; return ADDSUB; }
"*"     { charCnt += yyleng; yylval.oper = charCnt; return MUL; }
","     { charCnt += yyleng; return COMMA; }
"["     { charCnt += yyleng; return LSBC; }  // Left Square BraCket
"]"     { charCnt += yyleng; return RSBC; }  // Right Square BraCket
"("     { charCnt += yyleng; return LBC; }   // Left BraCket
")"     { charCnt += yyleng; return RBC; }   // Right BraCket
"^T"    { charCnt += yyleng; return TP; }    // TransPose
[0-9]+  { charCnt += yyleng; yylval.ival = atoi(yytext); return NUM; }
.       { /* do nothing */ }

%%