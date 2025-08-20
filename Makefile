#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# define the Cpp and C compiler to use
CXX = g++
CC = gcc

# define any compile-time flags
CXXFLAGS    := -std=c++17 -Wall -Wextra -g
CFLAGS      := -Wall -Wextra -g

# define output directory
OUTPUT  := output

# define source directory
SRC     := src

# define include directory
INCLUDE := include

ifeq ($(OS),Windows_NT)
    MAIN        := main.exe
    SOURCEDIRS  := $(SRC)
    INCLUDEDIRS := $(INCLUDE)
    FIXPATH = $(subst /,\,$1)
    RM          := del /q /f
    MD  := mkdir
    LDFLAGS     :=
else
    MAIN        := main
    SOURCEDIRS  := $(shell find $(SRC) -type d)
    INCLUDEDIRS := $(shell find $(INCLUDE) -type d)
    FIXPATH = $1
    RM = rm -f
    MD  := mkdir -p
    LDFLAGS     := -lpthread -lm
endif

# define any directories containing header files other than /usr/include
INCLUDES    := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))


# define the Cpp source files
CPPSOURCES  := $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))
# Define the C source files
CSOURCES    := $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# define the C and Cpp source files
SOURCES     := $(CPPSOURCES) $(CSOURCES)

# define the C object files
OBJECTS     := $(SOURCES:.c=.o)
OBJECTS     := $(OBJECTS:.cpp=.o)

# define the dependency output files
DEPS        := $(OBJECTS:.o=.d)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN  := $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LDFLAGS)

# include all .d files
-include $(DEPS)

# this is a suffix replacement rule for building .o's and .d's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# -MMD generates dependency output files same name as the .o file
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -MMD $<  -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $< -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!
