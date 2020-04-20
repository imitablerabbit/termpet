DEBUG=1
DEBUG_FLAGS=$(if ${DEBUG},--debug,)

PKG_CONFIG_DEPS=ncurses form menu
CFLAGS=$(shell pkg-config --cflags ${PKG_CONFIG_DEPS}) ${DEBUG_FLAGS}
CLIBS=$(shell pkg-config --libs ${PKG_CONFIG_DEPS})

SRC_DIR=src
SRC_FILES=$(wildcard ${SRC_DIR}/*.c)
H_FILES=$(wildcard ${SRC_DIR}/*.h)

OUT_FILE=termpet

VALGRIND_OPTS=--trace-children=yes --track-fds=yes

${OUT_FILE}: ${SRC_FILES} ${H_FILES}
	gcc -o ${OUT_FILE} ${CFLAGS} ${CLIBS} ${SRC_FILES}

.PHONY: clean debug

clean:
	rm ${OUT_FILE}

debug: ${OUT_FILE}
	gdb ${OUT_FILE}

valgrind: ${OUT_FILE}
	valgrind ${VALGRIND_OPTS} --tool=memcheck --leak-check=full \
		--show-leak-kinds=all ./${OUT_FILE}
