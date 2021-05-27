# ======================================================
# | Variables
# ------------------------------------------------------
# |- common --------------------------------------------
APP_NAME := colorize
VERSION := 1.4.0-$(shell git rev-parse --short HEAD)

# Switch between implementations for release target.
# Available options are:
# - C   : uses regex.h, selected by default for portability
# - CXX : uses std::regex, incompatible with gcc<4.9.0
# - both: uses both of the above, can switch between them using
#         a command line parameter of the compiled application
# Example:
#   $ make clean
#   $ make release IMPL=CXX
IMPL ?= C

PREFIX ?= ~/.local
INSTALLDIR := ${DESTDIR}${PREFIX}/bin/

CXXFLAGS += -Wall -Wextra -pedantic -std=c++11
FLAGS_DEBUG   := -O0 -g -DDEBUG -DUSE_REGEX_H -DUSE_GLIBCXX_REGEX
FLAGS_RELEASE := -O3

HEADERS := $(shell find src/ -type f -name '*.hpp')
SOURCES := $(shell find src/ -type f -name '*.cpp')

ifeq (${IMPL}, C)
	FLAGS_RELEASE += -DUSE_REGEX_H
	HEADERS_RELEASE := $(filter-out src/ColorizeCxx.hpp, ${HEADERS})
	SOURCES_RELEASE := $(filter-out src/ColorizeCxx.cpp, ${SOURCES})
	VERSION_RELEASE := ${VERSION}-C
else ifeq (${IMPL}, CXX)
	FLAGS_RELEASE += -DUSE_GLIBCXX_REGEX
	HEADERS_RELEASE := $(filter-out src/ColorizePosix.hpp, ${HEADERS})
	SOURCES_RELEASE := $(filter-out src/ColorizePosix.cpp, ${SOURCES})
	VERSION_RELEASE := ${VERSION}-Cxx
else ifeq (${IMPL}, both)
	FLAGS_RELEASE += -DUSE_REGEX_H -DUSE_GLIBCXX_REGEX
	SOURCES_RELEASE := ${SOURCES}
	VERSION_RELEASE := ${VERSION}-C&Cxx
else
$(error Invalid value for IMPL: ${IMPL}. Valid values are: {C, CXX, both}.)
endif
FLAGS_DEBUG   += -DVERSION="\"${VERSION}-C&Cxx-debug\""
FLAGS_RELEASE += -DVERSION="\"${VERSION_RELEASE}\""

OBJECTS_DEBUG   := ${SOURCES:src/%.cpp=build/debug/%.o}
OBJECTS_RELEASE := ${SOURCES_RELEASE:src/%.cpp=build/release/%.o}

# |- testing -------------------------------------------
TEST_LIB := tests/lib

CATCH2_BASE_URL := https://raw.githubusercontent.com/catchorg/Catch2/devel/extras
CATCH2_HEADER := tests/lib/catch2/catch_amalgamated.hpp
CATCH2_SOURCE := tests/lib/catch2/catch_amalgamated.cpp
.PRECIOUS: tests/lib/catch2/%

TEST_HEADERS := $(shell find tests/ -type f -name '*.hpp') ${CATCH2_HEADER}
TEST_SOURCES := $(shell find tests/ -type f -name '*.cpp') ${CATCH2_SOURCE}
TEST_OBJECTS := ${TEST_SOURCES:tests/%.cpp=build/tests/%.o}
TEST_LIB_OBJECTS := build/tests/lib/catch2/catch_amalgamated.o
# TEST_SCRIPTS := $(shell find tests/ -type f -name 'test_*.py')

# ======================================================
# | Commands
# ------------------------------------------------------
.PHONY: debug
debug: target/debug/${APP_NAME}

.PHONY: release
release: target/release/${APP_NAME}

.PHONY: test
test: test-unit test-e2e

.PHONY: test-unit
test-unit: target/${APP_NAME}-tests
	$^

.PHONY: test-e2e
test-e2e: target/debug/${APP_NAME}
	if command -v pytest >/dev/null; then \
		DUT_PATH=$< pytest -vvv tests/; \
	else \
		DUT_PATH=$< python -m unittest discover -v -s tests/; \
	fi

.PHONY: install
install: target/release/${APP_NAME}
	mkdir -p ${INSTALLDIR}
	@#install -m 755 -D $^ ${INSTALLDIR}${APP_NAME}  # install is not very portable
	cp $^ ${INSTALLDIR}${APP_NAME}
	chmod 755 ${INSTALLDIR}${APP_NAME}

.PHONY: uninstall
uninstall:
	rm -rf ${INSTALLDIR}${APP_NAME}

.PHONY: clean
clean:
	rm -rf target/ build/

# ======================================================
# | Objects
# ------------------------------------------------------
build/tests/%.o: tests/%.cpp ${CATCH2_HEADER} ${CATCH2_SOURCE}
	@mkdir -p ${@D}
	${CXX} $(filter-out -std=c++11, ${CXXFLAGS}) ${FLAGS_DEBUG} ${CPPFLAGS} -c -o $@ $<

build/debug/%.o: src/%.cpp src/%.hpp
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} ${FLAGS_DEBUG} ${CPPFLAGS} -c -o $@ $<

build/release/%.o: src/%.cpp src/%.hpp
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} ${FLAGS_RELEASE} ${CPPFLAGS} -c -o $@ $<

# ======================================================
# | Test executable
# ------------------------------------------------------
target/${APP_NAME}-tests: ${TEST_OBJECTS} ${TEST_LIB_OBJECTS} $(filter-out build/debug/main.o, ${OBJECTS_DEBUG})  # TODO: add HEADERS here
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} ${FLAGS_DEBUG} ${CPPFLAGS} -o $@ $^

${TEST_LIB}/catch2/%:
	@mkdir -p ${@D}
	curl -fLo $@ ${CATCH2_BASE_URL}/$*

# ======================================================
# | Application executable
# ------------------------------------------------------
target/debug/${APP_NAME}: ${OBJECTS_DEBUG}
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} ${FLAGS_DEBUG} ${CPPFLAGS} -o $@ $^

target/release/${APP_NAME}: ${OBJECTS_RELEASE}
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} ${FLAGS_RELEASE} ${CPPFLAGS} -o $@ $^
