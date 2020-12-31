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
#include "agent.h"
#include "simpleReflexAgent.h"

struct simpleReflexAgentRule {
  struct simpleReflexAgentAction action;
};

struct simpleReflexAgentState {
  int dummy;
};

static struct simpleReflexAgentState *
interpretInput(
  struct simpleReflexAgentPercept *percept
){
  (void)percept;
  return (0);
}

static struct simpleReflexAgentRule *
ruleMatch(
  struct simpleReflexAgentState *state
 ,struct simpleReflexAgentRule *rules
){
  (void)state;
  return (rules);
}

void
simpleReflexAgent(
  struct agent *context
){
  static struct simpleReflexAgentRule rulesP;
  struct simpleReflexAgentPercept *perceptM;

  pthread_cleanup_push((void(*)(void*))free, context);
  pthread_cleanup_push((void(*)(void*))chanClose, context->percept);
  pthread_cleanup_push((void(*)(void*))chanClose, context->action);
  while (chanOp(0, context->percept, (void **)&perceptM, chanOpGet) == chanOsGet && perceptM) {
    struct simpleReflexAgentState *state;
    struct simpleReflexAgentAction *actionM;

    state = interpretInput(perceptM);
    free(perceptM);
    if ((actionM = malloc(sizeof (*actionM)))) {
      struct simpleReflexAgentRule *rule;

      rule = ruleMatch(state, &rulesP);
      memcpy(actionM, &rule->action, sizeof (*actionM));
      if (chanOp(0, context->action, (void **)&actionM, chanOpPut) != chanOsPut) {
        free(actionM);
        break;
      }
    }
  }
  pthread_cleanup_pop(1); /* chanClose(context->action) */
  pthread_cleanup_pop(1); /* chanClose(context->percept) */
  pthread_cleanup_pop(1); /* free(context) */
}
