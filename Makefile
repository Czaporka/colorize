PREFIX ?= ~/.local

APP_NAME := colorize
CXXFLAGS := -Wall -Wextra -O3
INSTALLDIR := ${DESTDIR}${PREFIX}/bin/


.PHONY: all
all: target/${APP_NAME}

.PHONY: install
install: target/${APP_NAME}
	mkdir -p ${INSTALLDIR}
	install -m 755 -D $^ ${INSTALLDIR}

.PHONY: uninstall
uninstall:
	rm -rf ${INSTALLDIR}${APP_NAME}

.PHONY: clean
clean:
	rm -rf target

target/${APP_NAME}: src/*
	mkdir -p target
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $^
