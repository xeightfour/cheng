# Compiler and flags
CXX = g++
DEPFLAGS = -MMD -MP
CXXFLAGS = -Wall -Wextra -std=c++23 -O2 -I$(INCDIR)
LINKLIBS = -lglfw -lGLEW -lGL -ljpeg

# Target and directories
TARGET = lol.out
BLDDIR = build
SRCDIR = src
INCDIR = $(SRCDIR)/include
LIBDIR = lib

# Source and object files
SOURCES = $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BLDDIR)/%.o,$(SOURCES))
DEPENDS = $(patsubst $(SRCDIR)/%.cpp,$(BLDDIR)/%.d,$(SOURCES))

# Phony targets
.PHONY: clean cleanall all run

# Default goal
.DEFAULT_GOAL := all

# Build all
all: $(TARGET)

# Compile source files
$(BLDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $< -o $@ -c

# Link object files
$(TARGET): $(OBJECTS)
	$(CXX) $(LINKLIBS) $^ -o $@

# Run the target
run: $(TARGET)
	./$(TARGET)

# Clean build directory
clean:
	rm -Rf $(BLDDIR)

# Clean all
cleanall:
	rm -Rf $(BLDDIR) lol.out

# Include dependency files
-include $(DEPENDS)
