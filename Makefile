uptime-record: uptime-record.c
	touch ~/.config/uptime-record
	gcc uptime-record.c -ouptime-record
	touch ~/.config/uptime-record

install: uptime-record.c uptime-record
	touch ~/.config/uptime-record
	cp uptime-record /usr/bin/uptime-record

uninstall:
	rm /usr/bin/uptime-record

clean:
	rm uptime-record
