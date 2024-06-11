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
#include "simpleReflexAgent.h"

struct simpleReflexAgentPercept *
simpleReflexAgentPerceptNew(
  void
){
  return (calloc(1, sizeof (struct simpleReflexAgentPercept)));
}

void
simpleReflexAgentPerceptFree(
  struct simpleReflexAgentPercept *o
){
  free(o);
}

struct simpleReflexAgentAction *
simpleReflexAgentActionNew(
  void
){
  return (calloc(1, sizeof (struct simpleReflexAgentAction)));
}

void
simpleReflexAgentActionFree(
  struct simpleReflexAgentAction *o
){
  free(o);
}

struct simpleReflexAgentState {
  int dummy;
};

static const struct simpleReflexAgentState *
interpretInput(
  const struct simpleReflexAgentPercept *percept
){
  (void)percept;
  return (0); /* dummy */
}

struct simpleReflexAgentRule {
  const struct simpleReflexAgentAction *action;
};

static const struct simpleReflexAgentRule *
ruleMatch(
  const struct simpleReflexAgentState *state
 ,const struct simpleReflexAgentRule *rules
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
  static const struct simpleReflexAgentAction actions[] = {
   {0} /* dummy */
  };
  static const struct simpleReflexAgentRule rules[] = {
   {&actions[0]}
  };
  struct simpleReflexAgentPercept *percept;

  while (chanOp(0, context->sensor, (void **)&percept, chanOpGet) == chanOsGet) {
    struct simpleReflexAgentAction *action;

    if (!(action = simpleReflexAgentActionNew()))
      break;
    memcpy(action, ruleMatch(interpretInput(percept), rules)->action, sizeof (*action));
    if (chanOp(0, context->actuator, (void **)&action, chanOpPut) != chanOsPut) {
      simpleReflexAgentActionFree(action);
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
simpleReflexAgent(
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
