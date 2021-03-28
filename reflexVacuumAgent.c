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
#include <string.h>
#include <pthread.h>
#include "chan.h"
#include "chanFifo.h"
#include "reflexVacuumAgent.h"

struct reflexVacuumAgentPercept *
reflexVacuumAgentPerceptNew(
  void
){
  return (calloc(1, sizeof (struct reflexVacuumAgentPercept)));
}

void
reflexVacuumAgentPerceptFree(
  struct reflexVacuumAgentPercept *o
){
  free(o);
}

struct reflexVacuumAgentAction *
reflexVacuumAgentActionNew(
  void
){
  return (calloc(1, sizeof (struct reflexVacuumAgentAction)));
}

void
reflexVacuumAgentActionFree(
  struct reflexVacuumAgentAction *o
){
  free(o);
}

struct impl {
  chan_t *sensor;
  chan_t *actuator;
};

static void
impl(
  struct impl *context
){
  struct reflexVacuumAgentPercept *percept;

  while (chanOp(0, context->sensor, (void **)&percept, chanOpGet) == chanOsGet) {
    struct reflexVacuumAgentAction *action;

    if (!(action = reflexVacuumAgentActionNew()))
      break;
    if (percept->status == statusDirty)
      action->action = actionSuck;
    else if (percept->location == locationA)
      action->action = actionRight;
    else if (percept->location == locationB)
      action->action = actionLeft;
    else
      abort(); /* not in pseudocode */
    reflexVacuumAgentPerceptFree(percept);
    if (chanOp(0, context->actuator, (void **)&action, chanOpPut) != chanOsPut) {
      reflexVacuumAgentActionFree(action);
      break;
    }
  }
  chanShut(context->sensor);
  chanClose(context->sensor);
  chanShut(context->actuator);
  chanClose(context->actuator);
  free(context);
}
  
int
reflexVacuumAgent(
  chan_t **sensor
 ,chan_t **actuator
){
  struct impl *context;
  void *fifo;
  pthread_t p;

  if (!sensor || !actuator)
    return (1);
  *sensor = 0;
  *actuator = 0;
  if (!(fifo = chanFifoDySa((void(*)(void *))reflexVacuumAgentPerceptFree, 10, 1))
   || !(*sensor = chanCreate(chanFifoDySi, fifo, chanFifoDySd))
   || !(fifo = chanFifoDySa((void(*)(void *))reflexVacuumAgentPerceptFree, 10, 1))
   || !(*actuator = chanCreate(chanFifoDySi, fifo, chanFifoDySd))
   || !(context = malloc(sizeof (*context)))) {
    chanClose(*sensor);
    chanClose(*actuator);
    *sensor = 0;
    *actuator = 0;
    return (2);
  }
  context->sensor = chanOpen(*sensor);
  context->actuator = chanOpen(*actuator);
  if (pthread_create(&p, 0, (void *(*)(void *))impl, context)) {
    chanClose(context->sensor);
    chanClose(context->actuator);
    chanClose(*sensor);
    chanClose(*actuator);
    *sensor = 0;
    *actuator = 0;
    free(context);
    return (2);
  }
  pthread_detach(p);
  return (0);
}
