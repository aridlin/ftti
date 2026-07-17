CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
BUILD_DIR ?= build
FTUI_CFLAGS := $(shell pkg-config --cflags cairo x11 2>/dev/null)
FTUI_LIBS := $(shell pkg-config --libs cairo x11 2>/dev/null)

.PHONY: all clean

all: $(BUILD_DIR)/demo $(BUILD_DIR)/showcase $(BUILD_DIR)/minimal $(BUILD_DIR)/compat_ftui

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/demo: demo.cpp ftti.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) demo.cpp -o $@

$(BUILD_DIR)/showcase: examples/showcase.cpp ftti.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) examples/showcase.cpp -o $@

$(BUILD_DIR)/minimal: examples/minimal.cpp ftti.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) examples/minimal.cpp -o $@

$(BUILD_DIR)/compat_ftui: examples/compat_ftui.cpp ftti.hpp ftui.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(FTUI_CFLAGS) examples/compat_ftui.cpp -o $@ $(FTUI_LIBS)

clean:
	rm -rf $(BUILD_DIR)
