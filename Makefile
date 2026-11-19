CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -Iinclude
TARGET = pixel-forge
TEST_TARGET = test_image
SRCS = src/main.cpp src/image.cpp
LIB_SRCS = src/image.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

src/%.o: src/%.cpp include/image.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(TEST_TARGET): tests/test_image.cpp $(LIB_SRCS) include/image.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ tests/test_image.cpp $(LIB_SRCS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe $(TEST_TARGET) $(TEST_TARGET).exe
