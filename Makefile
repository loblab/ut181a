# UT181A makefile
# loblab, 5/13/2017

TARGET=ut181a
ARCH=$(shell uname -m)
OUTDIR ?= ./bin/$(ARCH)
TMPDIR ?= ./tmp/$(ARCH)
SRCDIR = ./src

all: $(TARGET)

#MYFLAGS ?= -D_DEBUG

CC       ?= gcc
CFLAGS   ?= -Wall -g $(ARCHFLAG) $(MYFLAGS)

CXX      ?= g++
CXXFLAGS ?= -Wall -g $(ARCHFLAG) $(MYFLAGS)

COBJS     = 
CPPOBJS   = $(addprefix $(TMPDIR)/, debug.o reader.o writer.o packet.o cp211x.o ut181a.o main.o)
OBJS      = $(COBJS) $(CPPOBJS)
LDFLAGS  ?= -L/usr/local/lib
LIBS     ?= -lslabhidtouart -lslabhiddevice
INCLUDES ?= -Iinc

$(TARGET): $(OUTDIR) $(OBJS) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $(OUTDIR)/$(TARGET)
	
$(COBJS): $(TMPDIR)
$(COBJS): $(TMPDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

$(CPPOBJS): $(TMPDIR)
$(CPPOBJS): $(TMPDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) $< -o $@

$(TMPDIR):
	mkdir -p $(TMPDIR)

$(OUTDIR):
	mkdir -p $(OUTDIR)

clean:
	rm -rf $(TMPDIR)
	rm -rf $(OUTDIR)

.PHONY: clean

