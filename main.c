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
#include "chan.h"
#include "agent.h"
#include "reflexVacuumAgent.h"

int
main(
){
  chan_t *percept;
  chan_t *action;
  struct reflexVacuumAgentPercept *perceptM;
  struct reflexVacuumAgentAction *actionM;
  int ret;

  chanInit(realloc, free);

  percept = action = 0;
  ret = 3;
  if (!(percept = chanCreate(0,0,0))
   || !(action = chanCreate(0,0,0))
   || (ret = agent(reflexVacuumAgent, percept, action)))
    goto exit;
  ret = 1;

  if (!(perceptM = calloc(1, sizeof (*perceptM))))
    goto exit;
  perceptM->status = statusDirty;
  if (chanOp(0, percept, (void **)&perceptM, chanOpPut) != chanOsPut
   || chanOp(0, action, (void **)&actionM, chanOpGet) != chanOsGet)
    goto exit;
  printf("percept Dirty action %s\n", reflexVacuumAgentActionAction[actionM->action]);
  free(actionM);

  if (!(perceptM = calloc(1, sizeof (*perceptM))))
    goto exit;
  perceptM->location = locationA;
  if (chanOp(0, percept, (void **)&perceptM, chanOpPut) != chanOsPut
   || chanOp(0, action, (void **)&actionM, chanOpGet) != chanOsGet)
    goto exit;
  printf("percept LocaitonA action %s\n", reflexVacuumAgentActionAction[actionM->action]);
  free(actionM);

  if (!(perceptM = calloc(1, sizeof (*perceptM))))
    goto exit;
  perceptM->location = locationB;
  if (chanOp(0, percept, (void **)&perceptM, chanOpPut) != chanOsPut
   || chanOp(0, action, (void **)&actionM, chanOpGet) != chanOsGet)
    goto exit;
  printf("percept LocaitonB action %s\n", reflexVacuumAgentActionAction[actionM->action]);
  free(actionM);

  ret = 0;
exit:
  chanShut(percept);
  chanClose(percept);
  chanClose(action);
  return (ret);
}
