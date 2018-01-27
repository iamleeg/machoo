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
#include <hurd/trivfs.h>

#include "machoo_defs.h"

// filesystem ops supported (i.e. none)
int trivfs_fstype = FSTYPE_MISC;
int trivfs_fsid = 0;
int trivfs_support_read = 0;
int trivfs_support_write = 0;
int trivfs_support_exec = 0;
int trivfs_allow_open = 0;
int trivfs_protid_nportclasses = 0;
int trivfs_cntl_nportclasses = 0;

// this is the real demuxer created by MIG
extern int machoo_server(
  mach_msg_header_t *InHeadP,
  mach_msg_header_t *OutHeadP);

// call through to trivfs for messages we don't support
static int demuxer(mach_msg_header_t *inp,
                   mach_msg_header_t *outp)
{
  return (machoo_server(inp, outp) || trivfs_demuxer(inp, outp));
}

// handle an incoming message!
kern_return_t machoo_msg_send(
  mach_port_t receiver,
  machoo_selector selector,
  mach_port_t *response,
  mach_msg_type_name_t *responsePoly
)
{
  // for the moment, prove that we're receiving the message
  fprintf(stderr, "[%d %s]\n", receiver, selector);
  // return self
  if (responsePoly != NULL)
  {
    *responsePoly = MACH_MSG_TYPE_MOVE_SEND;
  }
  if (response != NULL)
  {
    *response = receiver;
  }
  return ERR_SUCCESS;
}

int main(int argc, char **argv)
{
  int err;
  mach_port_t bootstrap;
  struct trivfs_control *fsys;

  // check in with the bootstrap server
  task_get_bootstrap_port(mach_task_self(), &bootstrap);
  if (bootstrap == MACH_PORT_NULL) {
    fprintf(stderr, "cannot retrieve bootstrap port\n");
    exit(-1);
  }
  // reply to parent and start our filesystem
  err = trivfs_startup(bootstrap,
                       0,
                       0,
                       0,
                       0,
                       0,
                       &fsys);
  if (err) {
    fprintf(stderr, "could not start trivfs\n");
    exit(-1);
  }

  // we don't need the bootstrap port any more
  mach_port_deallocate(mach_task_self(), bootstrap);

  /*
   * listen for clients and do the event loop.
   */
  ports_manage_port_operations_one_thread(fsys->pi.bucket,
                                          demuxer,
                                          0);

  return 0;
}

// these functions are needed by libtrivfs but don't do anything
void trivfs_modify_stat(struct trivfs_protid *cred, io_statbuf_t *st) {}
error_t trivfs_goaway(struct trivfs_control *fsys, int flags) { exit(0); }

