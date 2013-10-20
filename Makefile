###
# Compiler Options
CC	= clang

###
# Source Files

COLLECTION_SRC = $(wildcard src/Collection/*.c)
REGGIE_SRC     = $(wildcard src/Reggie/*.c)
SRC = $(COLLECTION_SRC) $(REGGIE_SRC)
OBJ = $(addprefix obj/, $(notdir $(SRC:.c=.o)))

###
# Test Files

TEST_SRC = $(wildcard testsrc/*.c)
TEST_PRG = $(addprefix test/, $(notdir $(TEST_SRC:.c=)))

###
# Compiler Flags

CFLAGS = -I ./include -std=c11 -Wall
LFLAGS = 

###
# Compile the Libraries

DYNAMIC = Reggie.so
STATIC  = Reggie.a

all: $(DYNAMIC) $(STATIC)

$(DYNAMIC): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@

$(STATIC): $(OBJ)
	$(AR) -rcs $@ $(OBJ)

obj/%.o: src/Reggie/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj/%.o: src/Collection/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj:
	mkdir -p obj

###
# Compile the Tests

test: $(TEST_PRG)

test/%: testsrc/%.c $(STATIC) | test_dir
	$(CC) $< $(STATIC) $(CFLAGS) $(LIBS) -o $@
	./$@

test_dir:
	mkdir -p test

###
# Clean

clean:
	rm -Rf $(OBJ) $(TEST_PRG) $(STATIC) $(DYNAMIC)

realclean: clean
	rm -Rf ./obj ./test
