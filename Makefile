build: 
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -O2 -o bin/colpak	

install: build
	install -D bin/colpak /usr/bin
	if ! [ -f /etc/cccp.conf ]; then install -D cccp.conf /etc/cccp.conf; fi

uninstall:
	rm -rf /usr/bin/cccp && rm -rf /etc/cccp.conf && rm -rf /var/cccp/data

clean:
	rm -rf bin/*
