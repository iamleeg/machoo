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

#include <pthread/pthread.h>

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
extern int machoo_class_server(
  mach_msg_header_t *InHeadP,
  mach_msg_header_t *OutHeadP);

// call through to trivfs for messages we don't support
static int demuxer(mach_msg_header_t *inp,
                   mach_msg_header_t *outp)
{
  return (machoo_class_server(inp, outp) || trivfs_demuxer(inp, outp));
}

extern void *null_object(void *port_bucket);

// create an object!
kern_return_t machoo_create_object(
  mach_port_t receiver,
  mach_port_t *object,
  mach_msg_type_name_t *objectPoly
)
{
  // for the moment, log the request
  fprintf(stderr, "creating a null object\n");
  // create a port bucket
  struct port_bucket *bucket = ports_create_bucket();
  fprintf(stderr, "bucket\n");
  // create a port class
  struct port_class *obj_class = ports_create_class(NULL, NULL);
  fprintf(stderr, "class\n");
  // add a port to the bucket
  struct port_info *obj_port;
  error_t port_error = ports_create_port(obj_class,
                                         bucket,
                                         sizeof(struct port_info),
                                         &obj_port);
  if (port_error != ERR_SUCCESS) {
    fprintf(stderr, "error creating port %d\n", port_error);
    return KERN_FAILURE;
  }
  fprintf(stderr, "port\n");
  // acquire send rights
  *object = ports_get_right(obj_port);
  fprintf(stderr, "receive right\n");
  // move send rights to the client
  *objectPoly = MACH_MSG_TYPE_MAKE_SEND;
  // start the thread!
  int thread_created;
  do {
    pthread_t unused;
    thread_created = pthread_create(&unused, NULL, null_object, bucket);
  } while (thread_created == EAGAIN);

  fprintf(stderr, "start thread\n");
  return (thread_created == 0) ? ERR_SUCCESS : KERN_FAILURE;
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

