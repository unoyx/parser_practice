
PWD:=$(shell pwd)
LLVM_CONFIG:=/Users/unoyx/src/llvm/build/bin/llvm-config
LD_FLAGS:=$(shell ${LLVM_CONFIG} --ldflags --system-libs --libs)

CPP_FLAGS=-Wall -std=c++11 -g -O0 $(shell ${LLVM_CONFIG} --cxxflags) -DDEBUG

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
DEP = $(obj:.o=.d)

-include $(DEP)

CC=clang++

lexer_test: lexer
	./lexer <lexer_cases/case1 >case1.out
	./lexer <lexer_cases/case2 >case2.out
	./lexer <lexer_cases/case3 >case3.out
	diff -q lexer_cases/case1.ref case1.out
	diff -q lexer_cases/case2.ref case2.out
	diff -q lexer_cases/case3.ref case3.out

lexer: CPP_FLAGS+= -DLEXER_TEST
lexer: $(OBJ)
	${CC} -o $@ $^ ${LD_FLAGS}

parser: CPP_FLAGS+= -DPARSER_TEST
parser: $(OBJ)
	${CC} -o $@ $^ ${LD_FLAGS}

%.o: %.cpp
	${CC} ${CPP_FLAGS} -o $@ -c $<

%.d: %.cpp
	@${CPP} ${CPP_FLAGS} $< -MT -MM $(@:.d=.o) >$@

.PHONY: clean

clean:
	-rm ${OBJ}
	-rm ./parser
	-rm ./lexer
	-rm *.out
	-rm -r *.dSYM
