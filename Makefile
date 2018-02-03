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

null: null.c null_object.c machoo_objectServer.c machoo_classServer.c
	$(CC) -D_GNU_SOURCE -o $@ $^ -ltrivfs -lports -lpthread

null_client: null_client.c machoo_objectUser.c machoo_classUser.c
	$(CC) -D_GNU_SOURCE -o $@ $^

machoo_classServer.c machoo_classUser.c machoo_class.h: machoo_class.defs
	mig machoo_class.defs

machoo_objectServer.c machoo_objectUser.c machoo_object.h: machoo_defs.h machoo_object.defs
	mig machoo_object.defs

.PHONY: clean_null
clean_null:
	-rm null null_client

.PHONY: clean_class
clean_class:
	-rm machoo_classServer.c machoo_classUser.c machoo_class.h

.PHONY: clean_object
clean_object:
	-rm machoo_objectServer.c machoo_objectUser.c machoo_object.h

.PHONY: clean
clean: clean_null clean_class clean_object

.PHONY: start_null
start_null: null
	settrans -ac /tmp/null null

.PHONY: stop_null
stop_null:
	settrans -fg /tmp/null

