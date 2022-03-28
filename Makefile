uptime-record: uptime-record.c
	gcc uptime-record.c -ouptime-record

install: uptime-record.c uptime-record
	cp uptime-record /usr/bin/uptime-record

uninstall:
	rm /usr/bin/uptime-record

clean:
	rm uptime-record