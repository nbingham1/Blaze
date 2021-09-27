CC = gcc
CXXFLAGS	= -O2 -g -fmessage-length=0 -std=c++11
SOURCES	:= $(shell find src -name '*.cpp') $(shell find src -name '*.c')
OBJECTS	:= $(subst .c,.o,$(subst .cpp,.o,$(subst src/,build/,$(SOURCES))))
DIRECTORIES := $(sort $(dir $(OBJECTS)))
INC_PATHS = -Isrc -Istdcore -Imathcore
LIB_PATHS = -Lstdcore -Lmathcore
LDFLAGS	= -lglfw -lGL -lGLU -lGLEW -lpthread -lstdcore -lmathcore
TARGET	= Blaze

all: build $(TARGET)

$(TARGET): $(OBJECTS)
	$(MAKE) -s $(MAKE_FLAGS) -C stdcore
	$(MAKE) -s $(MAKE_FLAGS) -C mathcore
	$(CXX) $(LIB_PATHS) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

build/%.o: src/%.cpp
	$(CXX) $(INC_PATHS) $(CXXFLAGS) -c -o $@ $<

build/%.o: src/%.c
	$(CC) $(INC_PATHS) $(CXXFLAGS) -c -o $@ $<

build:
	mkdir $(DIRECTORIES)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe
