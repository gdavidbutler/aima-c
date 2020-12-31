/*
 * aima-c - C implementation of algorithms from Russell And Norvig's "Artificial Intelligence - A Modern Approach"
 * Copyright (C) 2021 G. David Butler <gdb@dbSystems.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <pthread.h>
#include "chan.h"
#include "agent.h"

int
agent(
  void implementation(struct agent *)
 ,chan_t *percept
 ,chan_t *action
){
  struct agent *x;
  pthread_t p;
  
  if (!implementation
   || !percept
   || !action
   || !(x = malloc(sizeof (*x))))
    return (1);
  x->percept = chanOpen(percept);
  x->action = chanOpen(action);
  if (pthread_create(&p, 0, (void *(*)(void *))implementation, x)) {
    chanClose(x->percept);
    chanClose(x->action);
    free(x);
    return (2);
  }
  pthread_detach(p);
  return (0);
}
