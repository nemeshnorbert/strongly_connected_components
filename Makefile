# Primary compiler
CC := g++

SRCDIR := src
INCLUDEDIR = include
TESTDIR = test
BUILDDIR := build
TARGETDIR := bin

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name \*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TARGET := $(TARGETDIR)/main
TESTER := $(TARGETDIR)/tester
CFLAGS := -g -Wall
LIB :=
INC := -I $(INCLUDEDIR)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(LIB) $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -rvf $(BUILDDIR) $(TARGETDIR)/*;

tester:
	$(CC) $(CFLAGS) $(TESTDIR)/tester.cpp $(INC) $(LIB) -o $(TESTER);

.PHONY: all clean

