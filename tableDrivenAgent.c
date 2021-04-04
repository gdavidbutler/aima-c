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
#include "tableDrivenAgent.h"

struct tableDrivenAgentPercept *
tableDrivenAgentPerceptNew(
  void
){
  return (calloc(1, sizeof (struct tableDrivenAgentPercept)));
}

void
tableDrivenAgentPerceptFree(
  struct tableDrivenAgentPercept *o
){
  free(o);
}

struct tableDrivenAgentAction *
tableDrivenAgentActionNew(
  void
){
  return (calloc(1, sizeof (struct tableDrivenAgentAction)));
}

void
tableDrivenAgentActionFree(
  struct tableDrivenAgentAction *o
){
  free(o);
}

static const struct tableDrivenAgentAction *
lookup(
  struct tableDrivenAgentPercept **percepts
 ,unsigned int perceptsN
 ,const struct tableDrivenAgentAction *actions
){
  (void)percepts;
  (void)perceptsN;
  return (actions + 0); /* dummy */
}

struct impl {
  chan_t *sensor;
  chan_t *actuator;
};

static void
impl(
  struct impl *context
){
  static const struct tableDrivenAgentAction actions[] = {
   {0} /* dummy */
  };
  struct tableDrivenAgentPercept *percept;
  struct tableDrivenAgentPercept **percepts;
  unsigned int perceptsN;

  percepts = 0;
  perceptsN = 0;
  while (chanOp(0, context->sensor, (void **)&percept, chanOpGet) == chanOsGet) {
    struct tableDrivenAgentAction *action;
#if 0 /* this is what the pseudocode says to do */
    void *v;

    if (!(v = realloc(percepts, (perceptsN + 1) * sizeof (*percepts))))
      break;
    percepts = v;
    *(percepts + perceptsN) = percept;
    ++perceptsN;
#else /* but since this is dummy */
    tableDrivenAgentPerceptFree(percept);
#endif
    if (!(action = tableDrivenAgentActionNew()))
      break;
    memcpy(action, lookup(percepts, perceptsN, actions), sizeof (*action));
    if (chanOp(0, context->actuator, (void **)&action, chanOpPut) != chanOsPut) {
      tableDrivenAgentActionFree(action);
      break;
    }
  }
  free(percepts);
  chanShut(context->sensor);
  chanClose(context->sensor);
  chanShut(context->actuator);
  chanClose(context->actuator);
  free(context);
}
  
int
tableDrivenAgent(
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
