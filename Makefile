# ======================================================
# | Variables
# ------------------------------------------------------
# |- common --------------------------------------------
PREFIX ?= ~/.local

APP_NAME := colorize
CXXFLAGS := -Wall -Wextra -O3

HEADERS := $(shell find src/ -type f -name '*.hpp')
SOURCES := $(shell find src/ -type f -name '*.cpp')
OBJECTS := ${SOURCES:src/%.cpp=build/%.o}

# |- testing -------------------------------------------
TEST_LIB := test/lib

CATCH2_BASE_URL := https://raw.githubusercontent.com/catchorg/Catch2/devel/extras
CATCH2_HEADER := test/lib/catch2/catch_amalgamated.hpp
CATCH2_SOURCE := test/lib/catch2/catch_amalgamated.cpp
.PRECIOUS: test/lib/catch2/%

TEST_HEADERS := $(shell find test/ -type f -name '*.hpp') ${CATCH2_HEADER}
TEST_SOURCES := $(shell find test/ -type f -name '*.cpp') ${CATCH2_SOURCE}
TEST_OBJECTS := ${TEST_SOURCES:test/%.cpp=build/test/%.o}
TEST_LIB_OBJECTS := build/test/lib/catch2/catch_amalgamated.o

# ======================================================
# | Commands
# ------------------------------------------------------
.PHONY: all
all: target/${APP_NAME}

.PHONY: test
test: target/${APP_NAME}-test
	$^

.PHONY: install
install: target/${APP_NAME}
	install -m 755 -D $^ ${DESTDIR}${PREFIX}/bin/

.PHONY: uninstall
uninstall:
	rm -rf ${DESTDIR}${PREFIX}/bin/${APP_NAME}

.PHONY: clean
clean:
	rm -rf target/ build/

# ======================================================
# | Objects
# ------------------------------------------------------
build/test/%.o: test/%.cpp ${CATCH2_HEADER} ${CATCH2_SOURCE}
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c -o $@ $<

build/%.o: src/%.cpp src/%.hpp
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c -o $@ $<

# ======================================================
# | Test executable
# ------------------------------------------------------
target/${APP_NAME}-test: ${TEST_OBJECTS} ${TEST_LIB_OBJECTS} $(filter-out build/${APP_NAME}.o, ${OBJECTS})  # TODO: add HEADERS here
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $^

${TEST_LIB}/catch2/%:
	@mkdir -p ${@D}
	curl -fLo $@ ${CATCH2_BASE_URL}/$*

# ======================================================
# | Application executable
# ------------------------------------------------------
target/${APP_NAME}: ${OBJECTS}
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $^
