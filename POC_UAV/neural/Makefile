CXX=g++
CPPFLAGS=-I. -I./lib -O2  -std=c++11
LFDFLAGS=-L. -lcblas -lrefblas -lgfortran
SOURCES=$(wildcard *.cpp) $(wildcard lib/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=neural

.PHONY: all clean

all: .d $(SOURCES) $(TARGET)

.d: $(SOURCES)
	$(CXX) $(CPPFLAGS) -MM $(SOURCES) >.d
-include .d
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm $(OBJECTS)

icpc: CXX=icpc
icpc: all
