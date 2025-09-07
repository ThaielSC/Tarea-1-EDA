CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

OBJS = test.o utils.o matrix.o cluster.o simsearch.o

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
