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
#include "reflexVacuumAgent.h"

void
reflexVacuumAgent(
  struct agent *context
){
  struct reflexVacuumAgentPercept *perceptM;

  pthread_cleanup_push((void(*)(void*))free, context);
  pthread_cleanup_push((void(*)(void*))chanClose, context->percept);
  pthread_cleanup_push((void(*)(void*))chanClose, context->action);
  while (chanOp(0, context->percept, (void **)&perceptM, chanOpGet) == chanOsGet && perceptM) {
    struct reflexVacuumAgentAction *actionM;

    if ((actionM = calloc(1, sizeof (*actionM)))) {
      if (perceptM->status == statusDirty)
        actionM->action = actionSuck;
      else if (perceptM->location == locationA)
        actionM->action = actionRight;
      else if (perceptM->location == locationB)
        actionM->action = actionLeft;
      if (chanOp(0, context->action, (void **)&actionM, chanOpPut) != chanOsPut) {
        free(actionM);
        break;
      }
    }
    free(perceptM);
  }
  pthread_cleanup_pop(1); /* chanClose(context->action) */
  pthread_cleanup_pop(1); /* chanClose(context->percept) */
  pthread_cleanup_pop(1); /* free(context) */
}

const char *reflexVacuumAgentActionAction[] = { "Unknown", "Suck", "Right", "Left" };
