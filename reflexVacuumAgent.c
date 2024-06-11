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
    if (percept->status == reflexVacuumAgentPerceptStatusDirty)
      action->act = reflexVacuumAgentActionActSuck;
    else if (percept->location == reflexVacuumAgentPerceptLocationA)
      action->act = reflexVacuumAgentActionActMoveRight;
    else if (percept->location == reflexVacuumAgentPerceptLocationB)
      action->act = reflexVacuumAgentActionActMoveLeft;
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
  chan_t *sensor
 ,chan_t *actuator
){
  struct impl *context;
  pthread_t p;

  if (!sensor || !actuator)
    return (1);
  if (!(context = malloc(sizeof (*context))))
    return (2);
  context->sensor = chanOpen(sensor);
  context->actuator = chanOpen(actuator);
  if (pthread_create(&p, 0, (void *(*)(void *))impl, context)) {
    chanClose(context->sensor);
    chanClose(context->actuator);
    free(context);
    return (2);
  }
  pthread_detach(p);
  return (0);
}
