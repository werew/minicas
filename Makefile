
# Program name
TARGET = minicas

# Directories
OBJDIR = obj
BINDIR = bin
SRCDIR = src
INCDIR = include
LIBDIR = lib


# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -I $(INCDIR)

# Files
SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(INCDIR)/*.h
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o) 
DEP = $(OBJECTS:%.o=%.d)
DEPLIB = $(addprefix $(OBJDIR)/, grille.o jeu.o )
EXECUTABLE := $(BINDIR)/$(TARGET)
README = README.md



$(EXECUTABLE): $(OBJECTS)
	@echo "\n-----------------> Linking ... "
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

-include $(DEP) 		


# Dependencies to headers files are covered by the
# "-include $(DEP)" directive
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo "\n-----------------> Compiling $@ ... "
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -c $< -o $@




archive : $(TARGET).tar.gz

$(TARGET).tar.gz : $(INCDIR) $(SRCDIR) $(README) $(MAKEFILE_LIST)
	tar -cvzf $@ $^




clean:
	@echo "----------------- Cleaning -----------------"
	rm -f $(OBJECTS) $(EXECUTABLE) 



