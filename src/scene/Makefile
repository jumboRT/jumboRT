TARGET		:= tests
CC			:= clang++
CXX			:= clang++

SOURCES		:= cylinder_tests.cc simple_tests.cc
OBJECTS		:= cylinder_tests.o simple_tests.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) 

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

re:
	${MAKE} clean
	${MAKE}

