CXXFLAGS = -Wall -g -std=c++0x

OBJECTS = Instance.o Engine.o

default:	test1 example

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o $(OBJECTS) *~
Engine.o: Engine.cpp Engine.h PtrInterface.h Ptr.h
Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h
test1.o: test1.cpp Instance.h Engine.h PtrInterface.h Ptr.h
