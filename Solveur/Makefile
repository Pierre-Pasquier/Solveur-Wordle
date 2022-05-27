########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
SNOW =  -DSNOW_ENABLED -g 

CC = clang
CXXFLAGS = -std=c11 -Wall -lm -Isnow 
LDFLAGS = 

# Makefile settings - Can be customized.
APPNAME = preTT
APPNAME2=solveur
EXT = .c
SRCDIR = src
SRCDIR2=src/solveur
OBJDIR = obj
OBJDIR2=obj/solveur

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
OBJ-TEST=$(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)-test/%.o)

# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)
DELOBJTEST=$(OBJ-TEST)

########################################################################
####################### Targets beginning here #########################
########################################################################

# Builds the app

test-$(APPNAME) : $(OBJ-TEST)
	$(CC) $(CXXFLAGS) $(SNOW) -o $@ $^ $(LDFLAGS)

$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)-test/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) $(SNOW) -o $@ -c $<

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DELOBJTEST) $(APPNAME) test-$(APPNAME)
