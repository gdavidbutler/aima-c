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
#include "tableDrivenAgent.h"
#include "reflexVacuumAgent.h"
#include "simpleReflexAgent.h"
#include "modelBasedReflexAgent.h"

static void
tableDrivenAgentActuatorPrint(
  chan_t *actuator
){
  struct tableDrivenAgentAction *action;

  while (chanOp(0, actuator, (void **)&action, chanOpGet) == chanOsGet) {
    printf("tableDrivenAgentAction:\n");
    tableDrivenAgentActionFree(action);
  }
  chanClose(actuator);
}

static void
reflexVacuumAgentActuatorPrint(
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

static void
simpleReflexAgentActuatorPrint(
  chan_t *actuator
){
  struct simpleReflexAgentAction *action;

  while (chanOp(0, actuator, (void **)&action, chanOpGet) == chanOsGet) {
    printf("simpleReflexAgentAction:\n");
    simpleReflexAgentActionFree(action);
  }
  chanClose(actuator);
}

static void
modelBasedReflexAgentActuatorPrint(
  chan_t *actuator
){
  struct modelBasedReflexAgentAction *action;

  while (chanOp(0, actuator, (void **)&action, chanOpGet) == chanOsGet) {
    printf("modelBasedReflexAgentAction:\n");
    modelBasedReflexAgentActionFree(action);
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
  pthread_t p[4]; /* main thread has exit last, to wait for the printing threads */

  /* as required by chan.h */
  chanInit(realloc, free);

  /* main thread and Print threads constitute an environment */

  /* create sensor and actuator */
  if (!(sensor = chanCreate(0, 0, (chanSd_t)tableDrivenAgentPerceptFree))
   || !(actuator = chanCreate(0, 0, (chanSd_t)tableDrivenAgentActionFree))) {
    fprintf(stderr, "chanFifoDySa/chanCreate failed\n");
    return (1);
  }

  /* create agent */
  if ((i = tableDrivenAgent(sensor, actuator))) {
    fprintf(stderr, "tableDrivenAgent failed: %d\n", i);
    return (1);
  }

  /* give actuator to a thread to get actions from agent */
  if (pthread_create(&p[0], 0, (void *(*)(void *))tableDrivenAgentActuatorPrint, actuator)) {
    fprintf(stderr, "pthread_create failed\n");
    return (1);
  }

  /* put percepts on agents' sensor */
  {
    struct tableDrivenAgentPercept *percept;

    if (!(percept = tableDrivenAgentPerceptNew())) {
      fprintf(stderr, "tableDrivenAgentPerceptNew failed\n");
      return (1);
    }
    if (chanOp(0, sensor, (void **)&percept, chanOpPut) != chanOsPut) {
      fprintf(stderr, "chanOp failed\n");
      return (1);
    }
  }

  /* inform the agent there will be no more percepts */
  chanShut(sensor);
  chanClose(sensor);

  /* rinse and repeat */

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

  if ((i = reflexVacuumAgent(sensor, actuator))) {
    fprintf(stderr, "reflexVacuumAgent failed: %d\n", i);
    return (1);
  }

  if (pthread_create(&p[1], 0, (void *(*)(void *))reflexVacuumAgentActuatorPrint, actuator)) {
    fprintf(stderr, "pthread_create failed\n");
    return (1);
  }

  {
    struct reflexVacuumAgentPercept *percept;

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

  chanShut(sensor);
  chanClose(sensor);

  if (!(sensor = chanCreate(0, 0, (chanSd_t)simpleReflexAgentPerceptFree))
   || !(actuator = chanCreate(0, 0, (chanSd_t)simpleReflexAgentActionFree))) {
    fprintf(stderr, "chanFifoDySa/chanCreate failed\n");
    return (1);
  }

  if ((i = simpleReflexAgent(sensor, actuator))) {
    fprintf(stderr, "simpleReflexAgent failed: %d\n", i);
    return (1);
  }

  if (pthread_create(&p[2], 0, (void *(*)(void *))simpleReflexAgentActuatorPrint, actuator)) {
    fprintf(stderr, "pthread_create failed\n");
    return (1);
  }

  {
    struct simpleReflexAgentPercept *percept;

    if (!(percept = simpleReflexAgentPerceptNew())) {
      fprintf(stderr, "simpleReflexAgentPerceptNew failed\n");
      return (1);
    }
    if (chanOp(0, sensor, (void **)&percept, chanOpPut) != chanOsPut) {
      fprintf(stderr, "chanOp failed\n");
      return (1);
    }
  }

  chanShut(sensor);
  chanClose(sensor);

  if (!(sensor = chanCreate(0, 0, (chanSd_t)modelBasedReflexAgentPerceptFree))
   || !(actuator = chanCreate(0, 0, (chanSd_t)modelBasedReflexAgentActionFree))) {
    fprintf(stderr, "chanFifoDySa/chanCreate failed\n");
    return (1);
  }

  if ((i = modelBasedReflexAgent(sensor, actuator))) {
    fprintf(stderr, "modelBasedReflexAgent failed: %d\n", i);
    return (1);
  }

  if (pthread_create(&p[3], 0, (void *(*)(void *))modelBasedReflexAgentActuatorPrint, actuator)) {
    fprintf(stderr, "pthread_create failed\n");
    return (1);
  }

  {
    struct modelBasedReflexAgentPercept *percept;

    if (!(percept = modelBasedReflexAgentPerceptNew())) {
      fprintf(stderr, "modelBasedReflexAgentPerceptNew failed\n");
      return (1);
    }
    if (chanOp(0, sensor, (void **)&percept, chanOpPut) != chanOsPut) {
      fprintf(stderr, "chanOp failed\n");
      return (1);
    }
  }

  chanShut(sensor);
  chanClose(sensor);

  /* main thread must exit last */
  pthread_join(p[0], 0);
  pthread_join(p[1], 0);
  pthread_join(p[2], 0);
  pthread_join(p[3], 0);

  return (0);
}
