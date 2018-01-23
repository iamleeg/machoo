#
# Copyright 2018 Graham Lee.
#
# This file is part of machoo.
#
# Machoo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Machoo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

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

