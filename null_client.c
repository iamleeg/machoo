#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>
#include <hurd/hurd_types.h>

#include "machoo.h"

int main(int argc, char *argv[])
{
  mach_port_t null_object;
  mach_port_t returned_object;
  kern_return_t retVal;

  null_object = file_name_lookup("/tmp/null", 0, 0);
  printf("null object port: %u\n", null_object);

  retVal = machoo_msg_send(null_object, "doNothing", &returned_object);
  if (retVal != 0) {
    fprintf(stderr, "error sending message: %d\n", retVal);
    exit(-1);
  }
  printf("returned object from [%u doNothing]: %u\n", null_object, returned_object);

  return 0;
}

