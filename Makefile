machooServer.c machooUser.c machoo.h: machoo_defs.h message_send.defs
	mig message_send.defs

.PHONY: clean
clean:
	rm machooServer.c machooUser.c machoo.h

