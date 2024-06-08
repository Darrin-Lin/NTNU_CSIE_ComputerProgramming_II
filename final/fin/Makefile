CC=gcc
CFLAGS= -g -Wall -Wextra -fsanitize=address -lubsan -lasan
LDLIBS= -lm
obj = myvector.c mybmp.c
DBG =
all:
	-${CC} ${CFLAGS} ${LDLIBS} fin01.c ${obj} -o fin01 ${DBG}
	-${CC} ${CFLAGS} ${LDLIBS} fin02.c ${obj} -o fin02 ${DBG}
	-${CC} ${CFLAGS} ${LDLIBS} fin03.c ${obj} database.c -o fin03 ${DBG}

clean:
	rm -f fin01 fin02 fin03