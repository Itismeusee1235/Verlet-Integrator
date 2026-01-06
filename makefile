
CXX = g++
# Added -MMD and -MP flags for dependency generation
CXXFLAGS = -Iinclude -Wall -Wextra -O2 -std=c++17 -MMD -MP 
LIBS = -lSDL2 -lSDL2_image

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
# This lists the .d files that will be generated
DEPS = $(OBJ:.o=.d)
TARGET = sim

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# This line includes the generated dependency rules
-include $(DEPS)

clean:
	rm -f src/*.o src/*.d $(TARGET)
