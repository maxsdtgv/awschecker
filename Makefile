CC=g++
CFLAGS=-c -Wall -lstdc++
SOURCES=awschecker.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=awschecker

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@