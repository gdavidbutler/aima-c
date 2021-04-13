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

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "chan.h"
#include "chanFifo.h"
#include "reflexVacuumAgent.h"

static void
print(
  chan_t *actuator
){
  struct reflexVacuumAgentAction *action;

  while (chanOp(0, actuator, (void **)&action, chanOpGet) == chanOsGet) {
    printf("reflexVacuumAgentAction: %s\n"
    ,action->act == reflexVacuumAgentActionActSuck ? "Suck" :
     action->act == reflexVacuumAgentActionActMoveRight ? "Right" :
     action->act == reflexVacuumAgentActionActMoveLeft ? "Left" :
     "unknown");
    reflexVacuumAgentActionFree(action);
  }
  chanClose(actuator);
}

int
main(
  void
){
  chan_t *sensor;
  chan_t *actuator;
  int i;
  pthread_t p; /* to wait for print thread */

  /* as required by chan.h */
  chanInit(realloc, free);

  /* main thread and Print threads constitute an environment */

  /* create sensor and actuator */
  /* example of using stores greater than one to create sensor and actuator */
  {
    void *store;

    if (!(store = chanFifoDySa((void(*)(void *))reflexVacuumAgentPerceptFree, 10, 1))
     || !(sensor = chanCreate(chanFifoDySi, store, chanFifoDySd))
     || !(store = chanFifoDySa((void(*)(void *))reflexVacuumAgentActionFree, 10, 1))
     || !(actuator = chanCreate(chanFifoDySi, store, chanFifoDySd))) {
      fprintf(stderr, "chanFifoDySa/chanCreate failed\n");
      return (1);
    }
  }

  /* create agent */
  if ((i = reflexVacuumAgent(sensor, actuator))) {
    fprintf(stderr, "reflexVacuumAgent failed: %d\n", i);
    return (1);
  }

  /* give actuator to a thread to get actions from agent */
  if (pthread_create(&p, 0, (void *(*)(void *))print, actuator)) {
    fprintf(stderr, "pthread_create failed\n");
    return (1);
  }

  /* put percepts on agents' sensor */
  {
    struct reflexVacuumAgentPercept *percept;

    if (!(percept = reflexVacuumAgentPerceptNew())) {
      fprintf(stderr, "reflexVacuumAgentPerceptNew failed\n");
      return (1);
    }
    percept->location = reflexVacuumAgentPerceptLocationA;
    if (chanOp(0, sensor, (void **)&percept, chanOpPut) != chanOsPut) {
      fprintf(stderr, "chanOp failed\n");
      return (1);
    }

    if (!(percept = reflexVacuumAgentPerceptNew())) {
      fprintf(stderr, "reflexVacuumAgentPerceptNew failed\n");
      return (1);
    }
    percept->status = reflexVacuumAgentPerceptStatusDirty;
    if (chanOp(0, sensor, (void **)&percept, chanOpPut) != chanOsPut) {
      fprintf(stderr, "chanOp failed\n");
      return (1);
    }

    if (!(percept = reflexVacuumAgentPerceptNew())) {
      fprintf(stderr, "reflexVacuumAgentPerceptNew failed\n");
      return (1);
    }
    percept->location = reflexVacuumAgentPerceptLocationB;
    if (chanOp(0, sensor, (void **)&percept, chanOpPut) != chanOsPut) {
      fprintf(stderr, "chanOp failed\n");
      return (1);
    }
  }

  /* inform the agent there will be no more percepts */
  chanShut(sensor);
  chanClose(sensor);

  /* wait for print to exit */
  pthread_join(p, 0);

  return (0);
}
