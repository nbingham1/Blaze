CC = gcc
CXXFLAGS	 =  -O2 -g -fmessage-length=0
SOURCES	    :=  $(shell find src -name '*.cpp') $(shell find src -name '*.c')
OBJECTS	    :=  $(subst .c,.o,$(subst .cpp,.o,$(subst src/,build/,$(SOURCES))))
DIRECTORIES :=  $(sort $(dir $(OBJECTS)))
SEARCH_PATHS = "-IC:\\MinGW\\include" "-LC:\\MinGW\\bin" "-LC:\\MinGW\\lib"
LDFLAGS		 = -lopengl32 -lglut32 -lglu32 -lpthread -lgmp
TARGET		 =  Blaze

all: build $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(SEARCH_PATHS) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

build/%.o: src/%.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/%.o: src/%.c 
	$(CC) $(CXXFLAGS) -c -o $@ $<

build:
	mkdir $(DIRECTORIES)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe
