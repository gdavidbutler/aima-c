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
#include "modelBasedReflexAgent.h"

struct modelBasedReflexAgentPercept *
modelBasedReflexAgentPerceptNew(
  void
){
  return (calloc(1, sizeof (struct modelBasedReflexAgentPercept)));
}

void
modelBasedReflexAgentPerceptFree(
  struct modelBasedReflexAgentPercept *o
){
  free(o);
}

struct modelBasedReflexAgentAction *
modelBasedReflexAgentActionNew(
  void
){
  return (calloc(1, sizeof (struct modelBasedReflexAgentAction)));
}

void
modelBasedReflexAgentActionFree(
  struct modelBasedReflexAgentAction *o
){
  free(o);
}

struct modelBasedReflexAgentState {
  int dummy;
};

struct modelBasedReflexAgentTransitionModel {
  int dummy;
};

struct modelBasedReflexAgentSensorModel {
  int dummy;
};

static const struct modelBasedReflexAgentState *
updateState(
  struct modelBasedReflexAgentState **state
 ,struct modelBasedReflexAgentAction **action
 ,const struct modelBasedReflexAgentPercept *percept
 ,const struct modelBasedReflexAgentTransitionModel *transitionModel
 ,const struct modelBasedReflexAgentSensorModel *sensorModel
){
  (void)state;
  (void)action;
  (void)percept;
  (void)transitionModel;
  (void)sensorModel;
  return (0); /* dummy */
}

struct modelBasedReflexAgentRule {
  const struct modelBasedReflexAgentAction *action;
};

static const struct modelBasedReflexAgentRule *
ruleMatch(
  const struct modelBasedReflexAgentState *state
 ,const struct modelBasedReflexAgentRule *rules
){
  (void)state;
  return (rules + 0); /* dummy */
}

struct impl {
  chan_t *sensor;
  chan_t *actuator;
};

static void
impl(
  struct impl *context
){
  static const struct modelBasedReflexAgentAction actions[] = {
   {0} /* dummy */
  };
  static const struct modelBasedReflexAgentRule rules[] = {
   {&actions[0]}
  };
  struct modelBasedReflexAgentTransitionModel *transitionModel;
  struct modelBasedReflexAgentSensorModel *sensorModel;
  struct modelBasedReflexAgentState *state;
  struct modelBasedReflexAgentAction *lastAction;
  struct modelBasedReflexAgentPercept *percept;

  transitionModel = 0;
  sensorModel = 0;
  state = 0;
  lastAction = 0;
  while (chanOp(0, context->sensor, (void **)&percept, chanOpGet) == chanOsGet) {
    struct modelBasedReflexAgentAction *action;

    if (!(action = modelBasedReflexAgentActionNew()))
      break;
    memcpy(action, ruleMatch(updateState(&state, &lastAction, percept, transitionModel, sensorModel), rules)->action, sizeof (*action));
    if (chanOp(0, context->actuator, (void **)&action, chanOpPut) != chanOsPut) {
      modelBasedReflexAgentActionFree(action);
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
modelBasedReflexAgent(
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
