SRC_DIR := src
RELEASE_DIR := release
DEBUG_DIR := debug
INCLUDE_DIR := include
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
RELEASE_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(RELEASE_DIR)/%.o,$(SRC_FILES))
DEBUG_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(DEBUG_DIR)/%.o,$(SRC_FILES))
LDFLAGS := $(shell pkgconf --libs lua)
CPPFLAGS :=
CXXFLAGS := 

luamidi: $(RELEASE_OBJ_FILES)	
	g++ $(LDFLAGS) -o $@ $^

luamidid: $(DEBUG_OBJ_FILES) 
	g++ $(LDFLAGS) -o $@ $^

$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp | $(RELEASE_DIR)
	g++ -std=c++14 -I$(INCLUDE_DIR) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(RELEASE_DIR):
	mkdir -p $(RELEASE_DIR)

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp | $(DEBUG_DIR)
	g++ -g -DDEBUG -std=c++14 -I$(INCLUDE_DIR) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(DEBUG_DIR):
	mkdir -p $(DEBUG_DIR)

test1: luamidi
	./luamidi examples/test1.lua
clean:
	rm -r -f $(DEBUG_DIR)
	rm -r -f $(RELEASE_DIR)
	rm -f luamidi
	rm -f luamidid
