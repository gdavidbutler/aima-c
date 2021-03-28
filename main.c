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
#include "tableDrivenAgent.h"
#include "reflexVacuumAgent.h"

static void
tableDrivenAgentActuatorOutput(
  chan_t *actuator
){
  struct tableDrivenAgentAction *action;

  while (chanOp(0, actuator, (void **)&action, chanOpGet) == chanOsGet) {
    printf("tableDrivenAgentAction: %d\n", action->dummy);
    tableDrivenAgentActionFree(action);
  }
  chanClose(actuator);
}

static void
reflexVacuumAgentActuatorOutput(
  chan_t *actuator
){
  struct reflexVacuumAgentAction *action;

  while (chanOp(0, actuator, (void **)&action, chanOpGet) == chanOsGet) {
    printf("reflexVacuumAgentAction: %s\n"
    ,action->action == actionSuck ? "Suck" :
     action->action == actionRight ? "Right" :
     action->action == actionLeft ? "Left" :
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
  pthread_t p;

  chanInit(realloc, free);


  if ((i = tableDrivenAgent(&sensor, &actuator))) {
    fprintf(stderr, "tableDrivenAgent failed: %d\n", i);
    return (1);
  }
  if (pthread_create(&p, 0, (void *(*)(void *))tableDrivenAgentActuatorOutput, actuator)) {
    fprintf(stderr, "pthread_create failed\n");
    return (1);
  }
  {
    struct tableDrivenAgentPercept *percept;

    if (!(percept = calloc(1, sizeof (*percept)))) {
      fprintf(stderr, "calloc failed\n");
      return (1);
    }
    percept->dummy = 0;
    if (chanOp(0, sensor, (void **)&percept, chanOpPut) != chanOsPut) {
      fprintf(stderr, "chanOp failed\n");
      return (1);
    }
  }
  chanShut(sensor);
  chanClose(sensor);
  pthread_join(p, 0);


  if ((i = reflexVacuumAgent(&sensor, &actuator))) {
    fprintf(stderr, "reflexVacuumAgent failed: %d\n", i);
    return (1);
  }
  if (pthread_create(&p, 0, (void *(*)(void *))reflexVacuumAgentActuatorOutput, actuator)) {
    fprintf(stderr, "pthread_create failed\n");
    return (1);
  }
  {
    struct reflexVacuumAgentPercept *percept;

    if (!(percept = calloc(1, sizeof (*percept)))) {
      fprintf(stderr, "calloc failed\n");
      return (1);
    }
    percept->location = locationB;
    if (chanOp(0, sensor, (void **)&percept, chanOpPut) != chanOsPut) {
      fprintf(stderr, "chanOp failed\n");
      return (1);
    }
  }
  chanShut(sensor);
  chanClose(sensor);
  pthread_join(p, 0);


  return (0);
}
