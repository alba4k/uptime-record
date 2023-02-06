.PHONY: uptime-record

CC := gcc
ifeq ($(shell "whoami"),alba4k)
	CC := clang
endif

uptime-record: uptime-record.c
	mkdir -p ~/.local/share
	touch ~/.local/share/uptime-record
	$(CC) uptime-record.c -o uptime-record -Wall -Wextra -Ofast

run: uptime-record
	./uptime-record

install: uptime-record
	install uptime-record /usr/bin/uptime-record || echo -e "\e[31m\e[1mERROR\e[0m: Running without root proviliges?"

uninstall:
	rm -f /usr/bin/uptime-record

clean:
	rm uptime-record
