# Compiler and flags
CXX = g++
PYTHON_VERSION=3.10
PYTHON_INCLUDE = $(shell python$(PYTHON_VERSION) -c "from sysconfig import get_paths as gp; print(gp()['include'])")
NUMPY_INCLUDE = $(shell python$(PYTHON_VERSION) -c "import numpy; print(numpy.get_include())")

CXXFLAGS = -Wall -std=c++17 -O2 -I./ -I$(PYTHON_INCLUDE) -I$(NUMPY_INCLUDE)
LDFLAGS = -lpython$(PYTHON_VERSION)

# Targets and files
TARGET = quadtree
SRCS = main.cpp quadtree.cpp
OBJS = $(SRCS:.cpp=.o)
IMGS = wspd.png
# Build target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
