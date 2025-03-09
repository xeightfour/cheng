CXX = g++
DEPFLAGS = -MMD -MP
CXXFLAGS = -Wall -Wextra -std=c++23 -O2 -I$(INCDIR)
LINKLIBS = -lglfw -lGLEW -lGL -ljpeg

TARGET = lol.out
BLDDIR = build
SRCDIR = src
INCDIR = $(SRCDIR)/include
LIBDIR = lib
SOURCES = $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BLDDIR)/%.o,$(SOURCES))
DEPENDS = $(patsubst $(SRCDIR)/%.cpp,$(BLDDIR)/%.d,$(SOURCES))

.PHONY: clean cleanall all run

.DEFAULT_GOAL := all

all: $(TARGET)

$(BLDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $< -o $@ -c

$(TARGET): $(OBJECTS)
	$(CXX) $(LINKLIBS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -Rf $(BLDDIR)

cleanall:
	rm -Rf $(BLDDIR) lol.out

-include $(DEPENDS)
