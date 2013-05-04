
# DHBW CC directories
OBJDIR = bin
SRCDIR = src

# Executables
CC = gcc
BISON = bison
FLEX = flex
RM = rm
DOXYGEN = doxygen
SPLINT = splint
VALGRIND = valgrind

# Compiler Flags
STANDARDS = -m32 -std=gnu99 
INCLUDES = -I$(SRCDIR) -I$(OBJDIR) -I$(SRCDIR)/include
CFLAGS = -O0 -Wall -Wextra -pedantic -g $(INCLUDES) $(STANDARDS) -Wno-unused-parameter 
CFLAGS_GEN = -O2 -g -I$(OBJDIR) -w $(INCLUDES) $(STANDARDS)

.PHONY: all check test
all: compiler 
###############################################
# DHBW Compiler                               #
###############################################
DHBWCC_OBJECTS = $(addprefix $(OBJDIR)/, scanner.o parser.o main.o diag.o resource_manager.o symboltable.o ir_generator.o)

## Convenient targets for DHBW Compiler
.PHONY: compiler cc_objects cc_parser cc_scanner 
compiler: $(OBJDIR)/dhbwcc
cc_objects: $(GENERATED_OBJECTS)
parser: $(OBJDIR)/parser.c $(SRCDIR)/parser.h
scanner: $(OBJDIR)/scannSRCc


$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/dhbwcc: $(DHBWCC_OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $+

$(OBJDIR)/diag.o: $(SRCDIR)/diag.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/resource_manager.o: $(SRCDIR)/resource_manager.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/symboltable.o: $(SRCDIR)/symboltable.c $(SRCDIR)/symboltable.h
	$(CC) $(CFLAGS_GEN) -c $< -o $@

$(OBJDIR)/scanner.o: $(OBJDIR)/scanner.c $(SRCDIR)/parser.h
	$(CC) $(CFLAGS_GEN) -c $< -o $@

$(OBJDIR)/parser.o: $(OBJDIR)/parser.c $(SRCDIR)/parser.h
	$(CC) $(CFLAGS_GEN) -c $< -o $@

$(OBJDIR)/parser.c $(SRCDIR)/parser.h: $(SRCDIR)/parser.y $(OBJDIR)
	$(BISON) -v -t --locations --defines=$(SRCDIR)/parser.h \
	  -o $(OBJDIR)/parser.c -rall --report-file=$(OBJDIR)/bison.out $<

$(OBJDIR)/scanner.c: $(SRCDIR)/scanner.l $(SRCDIR)/parser.h
	$(FLEX) -o $(OBJDIR)/scanner.c $<
	
$(OBJDIR)/ir_generator.o: $(SRCDIR)/ir_generator.c $(SRCDIR)/ir_generator.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean_compiler
clean_compiler:
	$(RM) -f bin/*


###############################################
# DHBW Compiler RT-Check                      #
###############################################
SPLINT_OPTIONS = -unrecog
VALGRIND_OPTIONS = --leak-check=full

.PHONY: check_compiler
check_compiler:
	$(VALGRIND) $(VALGRIND_OPTIONS) bin/dhbwcc -p a.c

###############################################
# Doxygen Project Documentation               #
###############################################
DOXYGEN_CONFIG = doxygen/config
DOXYGEN_FLAGS = 

.PHONY: doxygen clean_doxygen
doxygen:
	$(DOXYGEN) $(DOXHGEN_FLAGS) $(DOXYGEN_CONFIG)

clean_doxygen:
	$(RM) -rf doxygen/html

.PHONY: clean
clean: clean_compiler clean_doxygen
	
