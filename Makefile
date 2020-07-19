PREFIX ?= ~/.local

APP_NAME := colorize-numerals
CXXFLAGS := -Wall -Wextra -O3


.PHONY: all
all: target/${APP_NAME}

.PHONY: install
install: target/${APP_NAME}
	install -m 755 -D $^ ${DESTDIR}${PREFIX}/bin/

.PHONY: uninstall
uninstall:
	rm -rf ${DESTDIR}${PREFIX}/bin/${APP_NAME}

.PHONY: clean
clean:
	rm -rf target

target/${APP_NAME}: src/*
	mkdir -p target
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $^
