all:
	flex tinyc.lex
	bison -d parser.y
	gcc -o a.out parser.tab.c traverseTree.c lex.yy.c -g -lfl

clean:
	rm -rf lex.yy.c parser.tab.c parser.tab.h a.out ASTtree.txt symbolTable.txt