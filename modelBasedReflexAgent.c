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
#include "modelBasedReflexAgent.h"

struct modelBasedReflexAgentRule {
  struct modelBasedReflexAgentAction action;
};

struct modelBasedReflexAgentState {
  int dummy;
};

struct modelBasedReflexAgentTransitionModel {
  int dummy;
};

struct modelBasedReflexAgentSensorModel {
  int dummy;
};

static struct modelBasedReflexAgentState *
updateState(
  struct modelBasedReflexAgentState *state
 ,struct modelBasedReflexAgentRule *action
 ,struct modelBasedReflexAgentPercept *percept
 ,struct modelBasedReflexAgentTransitionModel *transitionModel
 ,struct modelBasedReflexAgentSensorModel *sensorModel
){
  (void)state;
  (void)action;
  (void)percept;
  (void)transitionModel;
  (void)sensorModel;
  return (0);
}

static struct modelBasedReflexAgentRule *
ruleMatch(
  struct modelBasedReflexAgentState *state
 ,struct modelBasedReflexAgentRule *rules
){
  (void)state;
  return (rules);
}

void
modelBasedReflexAgent(
  struct agent *context
){
  static struct modelBasedReflexAgentTransitionModel transitionModelP;
  static struct modelBasedReflexAgentSensorModel sensorModelP;
  static struct modelBasedReflexAgentRule rulesP;
  struct modelBasedReflexAgentState *stateP;
  struct modelBasedReflexAgentRule *actionP;
  struct modelBasedReflexAgentPercept *perceptM;

  pthread_cleanup_push((void(*)(void*))free, context);
  pthread_cleanup_push((void(*)(void*))chanClose, context->percept);
  pthread_cleanup_push((void(*)(void*))chanClose, context->action);
  stateP = 0;
  actionP = 0;
  while (chanOp(0, context->percept, (void **)&perceptM, chanOpGet) == chanOsGet && perceptM) {
    struct modelBasedReflexAgentAction *actionM;

    stateP = updateState(stateP, actionP, perceptM, &transitionModelP, &sensorModelP);
    free(perceptM);
    if ((actionM = malloc(sizeof (*actionM)))) {
      struct modelBasedReflexAgentRule *rule;

      rule = ruleMatch(stateP, &rulesP);
      memcpy(actionM, &rule->action, sizeof (*actionM));
      if (chanOp(0, context->action, (void **)&actionM, chanOpPut) != chanOsPut) {
        free(actionM);
        break;
      }
    }
  }
  free(stateP);
  free(actionP);
  pthread_cleanup_pop(1); /* chanClose(context->action) */
  pthread_cleanup_pop(1); /* chanClose(context->percept) */
  pthread_cleanup_pop(1); /* free(context) */
}
