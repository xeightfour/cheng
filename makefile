CXX = g++

TARGET = lol.out
BLDDIR = build
SRCDIR = src
INCDIR = include

SOURCES = $(shell find $(SRCDIR) -name '*.cxx')

DEPFLAGS = -MMD -MP
CXXFLAGS = -Wall -std=c++20 -O2 -I$(INCDIR)
LINKLIBS = -lglfw -lGLEW -lGLU -lGL

OBJECTS = $(SOURCES:%=$(BLDDIR)/%.o)
DEPENDS = $(OBJECTS:.o=.d)

.PHONY: clean all

.DEFAULT_GOAL := all

all: $(TARGET)

$(BLDDIR)/%.cxx.o: %.cxx
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $< -o $@ -c

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LINKLIBS) $^ -o $@

clean:
	rm -Rf $(BLDDIR)

-include $(DEPENDS)
