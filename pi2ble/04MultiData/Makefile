# Makefile for multiple data input as overlay over video

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
OPENCV_FLAGS = `pkg-config --cflags --libs opencv4`

SOURCES = Frame.cpp \
	Main.cpp \
	App.cpp \

HEADERS = Frame.h \
	Config.h

TARGET = out

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(OPENCV_FLAGS)

clean: rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

help:
	@echo "help yourself"
	@echo " all		- Build the program"
	@echo " clean	- Remove compiled files"
	@echo " run		- Build and run program"
	@echo " help	- How did you even get here"
