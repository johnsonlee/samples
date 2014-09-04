CC=clang
CFLAGS=-framework Foundation

.PHONY: helloworld clean

helloworld: main.m
	@$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f helloworld

