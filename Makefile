.PHONY: all
all: null

null: null.c machooServer.c machoo.h machoo_defs.h
	$(CC) -D_GNU_SOURCE -o $@ $< machooServer.c -ltrivfs -lports

machooServer.c machooUser.c machoo.h: machoo_defs.h message_send.defs
	mig message_send.defs

.PHONY: clean
clean:
	rm machooServer.c machooUser.c machoo.h

