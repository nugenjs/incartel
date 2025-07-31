# Makefile for Multi-Input Data Visualization

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# OpenCV flags
OPENCV_FLAGS = `pkg-config --cflags --libs opencv4`

# Source files
SOURCES = 04MultiInput.cpp SensorSimulator.cpp DataVisualizer.cpp

# Header files (for dependency tracking)
HEADERS = SensorData.h SensorSimulator.h DataVisualizer.h

# Executable name
TARGET = 04MultiInput

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(OPENCV_FLAGS)

# Clean target
clean:
	rm -f $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Phony targets
.PHONY: all clean run debug help

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the program"
	@echo "  clean   - Remove compiled files"
	@echo "  run     - Build and run the program"
	@echo "  debug   - Build with debug symbols"
	@echo "  help    - Show this help message"

# Dependencies
04MultiInput.cpp: SensorSimulator.h DataVisualizer.h SensorData.h
SensorSimulator.cpp: SensorSimulator.h SensorData.h
DataVisualizer.cpp: DataVisualizer.h SensorData.h
