CXX := g++
CXX_FLAGS := -Wall -Wextra -std=c++20 -ggdb

BIN := bin
SRC := src
INCLUDE := include
LIB := lib
LIBRARIES := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
EXECUTABLE := main

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	@echo "Executing..."
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/Gui/*.cpp
	@echo "🚧 Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "🧹 Cleaning..."
	-rm $(BIN)/*