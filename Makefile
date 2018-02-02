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
# along with Machoo.  If not, see <http://www.gnu.org/licenses/>.

.PHONY: all
all: null null_client

null: null.c machoo_objectServer.c
	$(CC) -D_GNU_SOURCE -o $@ $^ -ltrivfs -lports

null_client: null_client.c machoo_objectUser.c
	$(CC) -D_GNU_SOURCE -o $@ $^

machoo_objectServer.c machoo_objectUser.c machoo.h: machoo_defs.h machoo_object.defs
	mig machoo_object.defs

.PHONY: clean
clean:
	rm machoo_objectServer.c machoo_objectUser.c machoo_object.h null null_client

.PHONY: start_null
start_null: null
	settrans -ac /tmp/null null

.PHONY: stop_null
stop_null:
	settrans -fg /tmp/null

