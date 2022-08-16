.PHONY: uptime-record

CC := gcc
ifeq ($(shell "whoami"),alba4k)
	CC := clang
endif

uptime-record: uptime-record.c
	touch ~/.local/share/uptime-record
	$(CC) uptime-record.c -o uptime-record -Wall -Wextra -Ofast

run: uptime-record
	./uptime-record

install: uptime-record
	cp uptime-record /usr/bin/uptime-record

uninstall:
	rm -f /usr/bin/uptime-record

clean:
	rm uptime-record
