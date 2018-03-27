
CFLAGS=-Wall -std=c++11 -g -O0

lexer_test: lexer
	./lexer <lexer_cases/case1 >case1.out
	./lexer <lexer_cases/case2 >case2.out
	./lexer <lexer_cases/case3 >case3.out
	diff -q lexer_cases/case1.ref case1.out
	diff -q lexer_cases/case2.ref case2.out
	diff -q lexer_cases/case3.ref case3.out

lexer:
	clang++ -DLEXER_TEST lexer.cpp main.cpp ${CFLAGS} -o lexer

clean:
	-rm ./lexer
	-rm *.out
	-rm -r *.dSYM
