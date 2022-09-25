CXX      :=
CXXFLAGS := -Wall -Wextra -Wno-unused-parameter -Wno-volatile -Wno-unused-but-set-variable -std=c++20
LDFLAGS  :=
BUILD    := build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)
SRC_DIR  := source
INCL     :=
TARGET   := program
SRC      := $(wildcard source/*.cpp)
EXT      :=

CXX += g++
LDFLAGS += -lstdc++ -lm `pkg-config --libs sdl2`
EXT += .out

OBJECTS  := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(SRC))

.PHONY: all debug release windows clean info

all: $(APP_DIR)/$(TARGET)$(EXT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCL) -c $< -MD -o $@

$(APP_DIR)/$(TARGET)$(EXT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET)$(EXT) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	@rm -f $(OBJECTS)
	@rm -f $(DEPENDENCIES)
	@echo "Cleanup complete!"

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
