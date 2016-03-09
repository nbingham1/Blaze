CC = gcc
CXXFLAGS	= -O2 -g -fmessage-length=0 -std=c++11
SOURCES	:= $(shell find src -name '*.cpp') $(shell find src -name '*.c')
OBJECTS	:= $(subst .c,.o,$(subst .cpp,.o,$(subst src/,build/,$(SOURCES))))
DIRECTORIES := $(sort $(dir $(OBJECTS)))
SEARCH_PATHS = -Isrc
LDFLAGS	= -lglfw -lGL -lGLU -lGLEW -lpthread
TARGET	= Blaze

all: build $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(SEARCH_PATHS) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

build/%.o: src/%.cpp
	$(CXX) $(SEARCH_PATHS) $(CXXFLAGS) -c -o $@ $<

build/%.o: src/%.c
	$(CC) $(SEARCH_PATHS) $(CXXFLAGS) -c -o $@ $<

build:
	mkdir $(DIRECTORIES)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe
