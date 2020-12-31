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

#include "chan.h"
#include "agent.h"
#include "tableDrivenAgent.h"
#include "reflexVacuumAgent.h"
#include "simpleReflexAgent.h"
#include "modelBasedReflexAgent.h"
#include "simpleProblemSolvingAgent.h"

int
main(
){
  chan_t *percept;
  chan_t *action;

  /* dummy example */
  percept = action = 0;
  if (!(percept = chanCreate(0,0,0))
   || !(action = chanCreate(0,0,0))
   || agent(tableDrivenAgent, percept, action))
    return (-1);

  /* connect action to something */
  /* connect percept to something */

  /* use the main thread for something or block for shutdown on action */
  chanClose(percept);
  chanClose(action);
  return (0);
}
