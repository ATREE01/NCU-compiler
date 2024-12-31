bison -d Mini-LISP.y
flex Mini-LISP.l
gcc -c ast.c
gcc -c Mini-LISP.tab.c
gcc -c lex.yy.c
gcc -o Mini-LISP ast.o lex.yy.o Mini-LISP.tab.o -lfl
rm lex.yy.c lex.yy.o Mini-LISP.tab.c Mini-LISP.tab.h Mini-LISP.tab.o ast.o

