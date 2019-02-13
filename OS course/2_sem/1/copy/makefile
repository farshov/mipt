CFLAGS=-MD -Wall -Wpedantic -Wshadow -Werror -std=c99 --coverage -g
.PHONY: all clean

all: a.out
-include *.d

a.out: prog.o test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm *.o *.d a.out *.gcno *.gcda *.gcov

