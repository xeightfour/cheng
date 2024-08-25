CXX = g++

TARGET = lol.out
BLDDIR = build
SRCDIR = src
INCDIR = $(SRCDIR)/include
LIBDIR = lib

SOURCES = $(shell find $(SRCDIR) -name '*.cxx')

DEPFLAGS = -MMD -MP
CXXFLAGS = -Wall -Wextra -std=c++23 -O2 -I$(INCDIR)
LINKLIBS = -lglfw -lGLEW -lGL -ljpeg

OBJECTS = $(SOURCES:%=$(BLDDIR)/%.o)
DEPENDS = $(OBJECTS:.o=.d)

.PHONY: clean cleanall all

.DEFAULT_GOAL := all

all: $(TARGET)

$(BLDDIR)/%.cxx.o: %.cxx
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $< -o $@ -c

$(TARGET): $(OBJECTS)
	$(CXX) $(LINKLIBS) $^ -o $@

clean:
	rm -Rf $(BLDDIR)

cleanall:
	rm -Rf $(BLDDIR) lol.out

-include $(DEPENDS)
