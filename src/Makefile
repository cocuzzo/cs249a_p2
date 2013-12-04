CXXFLAGS = -Wall -g -std=c++0x

OBJECTS = Instance.o Engine.o ActivityImpl.o

default: test1 example example2 test-cases-assign2 verification

test1: test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example: example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example2: example2.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	
test-cases-assign2: test-cases-assign2.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

verification: verification.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o example example.o example2 example2.o test-cases-assign2 test-cases-assign2.o $(OBJECTS) *~

ActivityImpl.o: ActivityImpl.cpp ActivityImpl.h	Activity.h 
Engine.o: Engine.cpp Engine.h PtrInterface.h Ptr.h
Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h
test1.o: test1.cpp Instance.h Engine.h PtrInterface.h Ptr.h Activity.h
test-cases-assign2.o: test-cases-assign2.cpp Instance.h Engine.h PtrInterface.h Ptr.h Activity.h
verification.o: verification.cpp Instance.h Activity.h