%{
#include <stdio.h>
#include <string.h>

// Variable to track the current indentation level
int indent_level = 0;

// Function to print indentation based on the current level
void print_indent() {
    for (int i = 0; i < indent_level; i++) {
        printf("\t"); 
    }
}
%}


lbr        \{              
rbr        \}              
str_with_lbr [^{}\n]*\{   
str_with_rbr [^{}\n]*\}    
str        [^{}\n]+        
newline    \n              
%option noyywrap 

%%

{lbr}        { print_indent(); indent_level++; printf("%s", yytext); }
{rbr}        { indent_level--; print_indent(); printf("%s", yytext); }
{str_with_lbr} { print_indent(); indent_level++; printf("%s", yytext); }
{str_with_rbr} { indent_level--; print_indent(); printf("%s", yytext); }
{str}       { print_indent(); printf("%s", yytext); }
{newline}   { printf("\n"); }

%%

// Main function to start the lexical analysis
int main(int argc, char *argv[]) {
    yylex();  // Start processing input
    return 0; // Exit program
}


// %{
//     int tabCount = 0;
//     void printTab()
//     {
//         for (int i = 0; i < tabCount; i++)
//         {
//             printf("\t");
//         }
//     }
// %}
// %%
// ^\{  { printTab(); tabCount++; ECHO; }
// \{  { tabCount++; ECHO; }
// ^\}  { tabCount--; printTab(); ECHO; }
// \}  { tabCount--; ECHO; }
// ^.  { printTab(); ECHO; }
// %%
// int main(int argc, char *argv[]) {
//     yylex();
//     return(0);
// }