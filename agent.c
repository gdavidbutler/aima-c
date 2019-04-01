/*
 * aima-c - C implementation of algorithms from Russell And Norvig's "Artificial Intelligence - A Modern Approach"
 * Copyright (C) 2019 G. David Butler <gdb@dbSystems.com>
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
#include "agent.h"

static void
agentFunction(
  agent_t *agent
){
  (void)*agent;
}

agent_t *
newAgent(
){
  agent_t *agent;

  if ((agent = malloc(sizeof(*agent)))) {
    agent->opaque = 0;
    agent->sensor.opaque = 0;
    agent->actuator.opaque = 0;
    agent->perceptSequence.percept = 0;
    agent->perceptSequence.perceptCount = 0;
    agent->function = agentFunction;
  }
  return agent;
}

void
deleteAgent(
  agent_t *self
){
  free(self);
}
