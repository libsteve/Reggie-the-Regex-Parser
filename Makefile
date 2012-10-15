###
# Compiler Options
CC	= clang

###
# Target Sources
LIST 		= src/list.c 		src/list.h
STRINGS 	= src/strings.c 	src/strings.h
NFA 		= src/nfa.c 		src/nfa.h
NFA_EVAL 	= src/nfa_eval.c 	src/nfa_eval.h

###
# Target Objects
LIST_O		= bin/build/list.o
STRINGS_O 	= bin/build/strings.o
NFA_O 		= bin/build/nfa.o
NFA_EVAL_O 	= bin/build/nfa_eval.o

# All NFA-Related Objects
ALL_NFA_O 	= $(LIST_O) $(STRINGS_O) $(NFA_O) $(NFA_EVAL_O)

###
# Suffixes
# .SUFFIXES: .RESULT .ORIGINAL
.SUFFIXES: .o .c

all-object: bin/build $(ALL_O)

###
# Set up the directories for building

bin/build:
	mkdir -p bin/build

testbin/build:
	mkdir -p testbin/build

###
# Build the object files for the source

$(LIST_O): $(LIST) bin/build
	$(CC) -c -o $(LIST_O) src/list.c

$(STRINGS_O): $(STRINGS) bin/build
	$(CC) -c -o $(STRINGS_O) src/strings.c

$(NFA_O): $(NFA) bin/build
	$(CC) -c -o $(NFA_O) src/nfa.c

$(NFA_EVAL_O): $(NFA_EVAL) bin/build
	$(CC) -c -o $(NFA_EVAL_O) src/nfa_eval.c

###
# Target Test Sources
TESTER 			= test/tester.c 		test/tester.h

LIST_TEST 		= test/list-test.c 		test/list-test.h
STRINGS_TEST 	= test/strings-test.c 	test/strings-test.h
NFA_TEST 		= test/nfa-test.c 		test/nfa-test.h
NFA_EVAL_TEST 	= test/nfa_eval-test.c 	test/nfa_eval-test.h

###
# Target Test Objects
TESTER_O		= testbin/build/tester.o

LIST_TEST_O		= testbin/build/list-test.o
STRINGS_TEST_O 	= testbin/build/strings-test.o
NFA_TEST_O 		= testbin/build/nfa-test.o
NFA_EVAL_TEST_O = testbin/build/nfa_eval-test.o

###
# Compile Tests

$(TESTER_O): $(TESTER)
	$(CC) -c -o $(TESTER_O) test/tester.c

testbin/list-test: testbin/build $(LIST_TEST) $(LIST_O) $(TESTER_O)
	$(CC) -c -o $(LIST_TEST_O) test/list-test.c
	$(CC) -o testbin/list-test $(LIST_TEST_O) $(LIST_O) $(TESTER_O)

testbin/nfa-test: ;
# testbin/nfa-test: test/nfa-test.c test/nfa-test.h bin/build/nfa.o testbin/build
# 	clang -c -o testbin/build/nfa-test.o test/nfa-test.c
# 	clang -o testbin/nfa-test testbin/build/nfa-test.o bin/build/nfa.o

testbin/strings-test: testbin/build $(STRINGS_TEST) $(STRINGS_O) $(TESTER_O)
	$(CC) -c -o $(STRINGS_TEST_O) test/strings-test.c
	$(CC) -o testbin/strings-test $(STRINGS_TEST_O) $(STRINGS_O) $(TESTER_O)

testbin/nfa_eval-test: testbin/build $(NFA_EVAL_TEST) $(ALL_NFA_O) $(TESTER_O)
	$(CC) -c -o $(NFA_EVAL_TEST_O) test/nfa_eval-test.c
	$(CC) -o testbin/nfa_eval-test $(ALL_NFA_O) $(ALL_NFA_O) $(TESTER_O)

###
# Compile and Run Tests

test: testbin/list-test testbin/nfa-test testbin/strings-test testbin/nfa_eval-test
	./testbin/list-test
	# ./testbin/nfa-test
	./testbin/strings-test
	./testbin/nfa_eval-test

###
# Remove build files from 

clean:
	rm -Rf bin/build
	rm -Rf testbin/build

real-clean:
	rm -Rf bin
	rm -Rf testbin