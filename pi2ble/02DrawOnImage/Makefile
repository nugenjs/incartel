# Makefile for OpenCV programs

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# OpenCV flags
OPENCV_FLAGS = `pkg-config --cflags --libs opencv4`

# Source files
SOURCES = 02DrawOnImage.cpp

# Executable name
TARGET = 02DrawOnImage

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(OPENCV_FLAGS)

# Clean target
clean:
	rm -f $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the program"
	@echo "  clean   - Remove compiled files"
	@echo "  run     - Build and run the program"
	@echo "  help    - Show this help message"
