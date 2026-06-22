# Makefile for Schwarzschild-BH-Sim (macOS / arm64)

CXX      := clang++
CXXFLAGS := -std=c++17 -O3 -ffast-math -arch arm64 -Wall -Iinclude -DGL_SILENCE_DEPRECATION

# GLFW static lib bundled in the repo
LIBDIR   := include/lib-arm64
LIBS     := $(LIBDIR)/libglfw3.a

# macOS frameworks required by GLFW + OpenGL
FRAMEWORKS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreFoundation

TARGET   := bh-sim
SOURCES  := Main.cpp Render.cpp
OBJECTS  := $(SOURCES:.cpp=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIBS) $(FRAMEWORKS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
