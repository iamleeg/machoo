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
#include <hurd/hurd_types.h>

#include "machoo_class.h"
#include "machoo_object.h"

int main(int argc, char *argv[])
{
  mach_port_t null_constructor;
  mach_port_t returned_object;
  kern_return_t retVal;

  null_constructor = file_name_lookup("/tmp/null", 0, 0);
  printf("null constructor port: %u\n", null_constructor);

  retVal = machoo_create_object(null_constructor, &returned_object);
  if (retVal != 0) {
    fprintf(stderr, "error creating object: %d\n", retVal);
    exit(-1);
  }
  printf("returned object from null server: %d\n", returned_object);

  retVal = machoo_msg_send(returned_object,
                           "doSomething");
  if (retVal != 0) {
    fprintf(stderr, "error sending message: %d\n", retVal);
    exit(-1);
  }

  return 0;
}

