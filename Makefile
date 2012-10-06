all-object: list.o nfa.o strings.o nfa_eval.o

all-test: list-test nfa-test strings-test nfa_eval-test

run-test: all-test
	./testbin/list-test
	./testbin/nfa-test
	./testbin/strings-test
	./testbin/nfa_eval-test

###
# Build the object files for the source

list.o: src/list.c src/list.h
	clang -c -o bin/build/list.o src/list.c

nfa.o: src/nfa.c src/nfa.h
	clang -c -o bin/build/nfa.o src/nfa.c

strings.o: src/strings.c src/strings.h
	clang -c -o bin/build/strings.o src/strings.c

nfa_eval.o: src/nfa_eval.c src/nfa_eval.h
	clang -c -o bin/build/nfa_eval.o src/nfa_eval.c

###
# Compile Tests

list-test:

nfa-test:

strings-test:

nfa_eval-test:

###
# Remove build files from 

clean:
	rm bin/build/*.o

real-clean:
	rm testbin/*
	rm -Rf bin/*
	mkdir bin/build