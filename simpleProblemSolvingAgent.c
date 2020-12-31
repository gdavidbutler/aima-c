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
#include "simpleProblemSolvingAgent.h"

struct simpleProblemSolvingAgentState {
  int dummy;
};

static struct simpleProblemSolvingAgentState *
updateState(
  struct simpleProblemSolvingAgentState *state
 ,struct simpleProblemSolvingAgentPercept *percept
){
  (void)state;
  (void)percept;
  return (0);
}

struct simpleProblemSolvingAgentGoal {
  int dummy;
};

static struct simpleProblemSolvingAgentGoal *
formulateGoal(
  struct simpleProblemSolvingAgentState *state
){
  (void)state;
  return (0);
}

struct simpleProblemSolvingAgentProblem {
  int dummy;
};

static struct simpleProblemSolvingAgentProblem *
formulateProblem(
  struct simpleProblemSolvingAgentState *state
 ,struct simpleProblemSolvingAgentGoal *goal
){
  (void)state;
  (void)goal;
  return (0);
}

static struct simpleProblemSolvingAgentAction **
search(
  struct simpleProblemSolvingAgentProblem *problem
){
  (void)problem;
  return (0);
}

void
simpleProblemSolvingAgent(
  struct agent *context
){
  struct simpleProblemSolvingAgentState *stateP;
  struct simpleProblemSolvingAgentPercept *perceptM;

  pthread_cleanup_push((void(*)(void*))free, context);
  pthread_cleanup_push((void(*)(void*))chanClose, context->percept);
  pthread_cleanup_push((void(*)(void*))chanClose, context->action);
  stateP = 0;
  while (chanOp(0, context->percept, (void **)&perceptM, chanOpGet) == chanOsGet && perceptM) {
    struct simpleProblemSolvingAgentGoal *goal;
    struct simpleProblemSolvingAgentProblem *problem;
    struct simpleProblemSolvingAgentAction **seq;
    struct simpleProblemSolvingAgentAction **p;

    stateP = updateState(stateP, perceptM);
    free(perceptM);
    goal = formulateGoal(stateP);
    problem = formulateProblem(stateP, goal);
    seq = search(problem);
    for (p = seq; *p; ++p) {
      struct simpleProblemSolvingAgentAction *actionM;

      if ((actionM = malloc(sizeof (*actionM)))) {
        memcpy(actionM, *p, sizeof (*actionM));
        if (chanOp(0, context->action, (void **)&actionM, chanOpPut) != chanOsPut) {
          free(actionM);
          break;
        }
      }
    }
    free(seq);
  }
  free(stateP);
  pthread_cleanup_pop(1); /* chanClose(context->action) */
  pthread_cleanup_pop(1); /* chanClose(context->percept) */
  pthread_cleanup_pop(1); /* free(context) */
}
