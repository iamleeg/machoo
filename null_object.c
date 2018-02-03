/*
 * Copyright 2018 Graham Lee.
 *
 * This file is part of machoo.
 *
 * Machoo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Machoo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Machoo.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include <hurd.h>
#include <hurd/ports.h>

#include "machoo_defs.h"

// this is the real demuxer created by MIG
extern int machoo_object_server(
  mach_msg_header_t *InHeadP,
  mach_msg_header_t *OutHeadP);

void *null_object(void *port_bucket)
{
  ports_manage_port_operations_one_thread(port_bucket,
                                          machoo_object_server,
                                          0);
  return NULL;
}

// handle an incoming message!
kern_return_t machoo_msg_send(
  mach_port_t receiver,
  machoo_selector selector
)
{
  // for the moment, prove that we're receiving the message
  fprintf(stderr, "[%lu %s]\n", receiver, selector);
  return ERR_SUCCESS;
}
