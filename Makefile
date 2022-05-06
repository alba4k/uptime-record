uptime-record: uptime-record.c
	touch ~/.config/uptime-record
	gcc uptime-record.c -ouptime-record -Wall

install: uptime-record
	cp uptime-record /usr/bin/uptime-record

uninstall:
	rm /usr/bin/uptime-record

clean:
	rm uptime-record