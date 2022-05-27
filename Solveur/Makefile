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


############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
OBJ-TEST=$(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)-test/%.o)

SRC2=$(wildcard $(SRCDIR2)/*$(EXT))
OBJ2= $(SRC2:$(SRCDIR2)/%$(EXT)=$(OBJDIR)/solveur/%.o)
OBJ2-TEST=$(SRC2:$(SRCDIR2)/%$(EXT)=$(OBJDIR)-test/solveur/%.o)

# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ) $(OBJ2)
DELOBJTEST=$(OBJ-TEST) $(OBJ2-TEST)

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

#####################################

#Construit le solveur

$(APPNAME2) : $(OBJ2)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

test-$(APPNAME2) : $(OBJ2-TEST)
	$(CC) $(CXXFLAGS) $(SNOW) -o $@ $^ $(LDFLAGS)

#####

#Règles de construction pour les .o

$(OBJDIR)/solveur/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)-test/solveur/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) $(SNOW) -o $@ -c $<


################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DELOBJTEST) $(APPNAME) test-$(APPNAME) $(APPNAME2) test-$(APPNAME2)
