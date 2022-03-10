# C++ compiler and compilation flags
CXX=g++
CXX_FLAGS= -g -Wall -ldl -pthread

# The demo executable, which contains `int main()`.
DEMO_DRIVER=demo.cpp

# $(wildcard) matches every file in the directory with a ".cpp" extension,
# then $(filter-out) removes the files that are already listed in TEST_SOURCES,
# as well as the driver module
DEMO_SOURCES=$(filter-out $(DEMO_DRIVER) $(TEST_SOURCES), $(wildcard *.cpp))

# TEST_SOURCES contains every file in the directory with a name like
# "test[...].cpp"
TEST_SOURCES=$(wildcard test*.cpp)

# "take every file in DEMO_SOURCES, and replace the *.cpp extension with *.o"
# note: equivalent to $(DEMO_SOURCES:%.cpp=%.o)
DEMO_OBJS=$(DEMO_SOURCES:.cpp=.o)

all: demo

# Compile the disk demo
demo: $(DEMO_OBJS) $(DEMO_DRIVER)
	$(CXX) $(CXX_FLAGS) -o $@ $^ $
	#        ^ name of the recipe, i.e. left of the `:`
	#           ^ the dependencies, i.e. everything right of the ":"

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $<

clean:
	rm -f $(DEMO_OBJS) *.out *.exe *.pch demo

# "Template" recipe for compiling a test file; any make target or dependency having
# the form "test[...].out" will use this recipe
test%.out: test%.cpp $(DEMO_SOURCES)
	$(CXX) -o $@ $^ $(CXX_FLAGS)

the dependencies for this target are:
#   "every file in TEST_SOURCES, with the '.cpp' extension replaced with '.out'"
alltests: $(TEST_SOURCES:%.cpp=%.out)
	echo "All tests compiled successfully!"
