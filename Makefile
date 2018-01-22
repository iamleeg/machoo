.PHONY: all
all: null

null: null.c machooServer.c
	$(CC) -D_GNU_SOURCE -o $@ $^ -ltrivfs -lports

null_client: null_client.c machooUser.c
	$(CC) -D_GNU_SOURCE -o $@ $^

machooServer.c machooUser.c machoo.h: machoo_defs.h message_send.defs
	mig message_send.defs

.PHONY: clean
clean:
	rm machooServer.c machooUser.c machoo.h null null_client

.PHONY: start_null
start_null: null
	settrans -ac /tmp/null null

.PHONY: stop_null
stop_null:
	settrans -fg /tmp/null

