CXX = g++

CXXFLAGS = -Wall -std=c++17 -O2 -Iinclude -MMD -MP
LINKLIBS = -lglfw -lGLEW -lGLU -lGL

SOURCES = src/main.cxx

OBJECTS = $(SOURCES:.cxx=.o)
DEPENDS = $(OBJECTS:.o=.d)

.PHONY: clean all
	.DEFAULT_GOAL := all

all: lol.out

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $< -o $@ -c

lol.out: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LINKLIBS) $^ -o $@

clean:
	rm -f src/*.o src/*.d

-include $(DEPENDS)
