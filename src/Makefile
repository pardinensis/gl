TARGET   := runme
CXXFLAGS := -g -Wextra -Wall -std=c++11
CXX      := g++
LIBS     := -lglfw3 -lX11 -lXxf86vm -lGL -lGLU -lGLEW -lXrandr -lpthread -lXi
EXT      := cpp

MAINDIR  := ..
BUILDDIR := ../build
 
override BUILDDIR := $(strip $(BUILDDIR))
SOURCES  := $(wildcard *.$(EXT))
OBJECTS  := $(patsubst %.$(EXT), $(BUILDDIR)/%.o, $(SOURCES))
DEPS     := $(patsubst %.$(EXT), $(BUILDDIR)/%.dep, $(SOURCES))
 
.PHONY: all
all: $(TARGET)
 
$(TARGET): $(OBJECTS) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $(MAINDIR)/$(TARGET) $(OBJECTS) $(LIBS)
 
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif
 
$(OBJECTS): $(BUILDDIR)/%.o: %.$(EXT) $(BUILDDIR)/%.dep $(BUILDDIR)/.tag
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
$(DEPS): $(BUILDDIR)/%.dep: %.$(EXT) $(BUILDDIR)/.tag
	mkdir -p $(dir $(@))
	$(CXX) $(CXXFLAGS) -MM $< -MT $@ -MT $(<:.$(EXT)=.o) -o $@
 
%.tag:
	mkdir -p $(dir $(@))
	touch $@
 
.PHONY: clean
clean:
	$(RM) $(MAINDIR)/$(TARGET)
	$(RM) -r $(BUILDDIR)