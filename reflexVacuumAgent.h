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

struct reflexVacuumAgentPercept {
  enum {
    reflexVacuumAgentPerceptLocationA
   ,reflexVacuumAgentPerceptLocationB
  } location;
  enum {
    reflexVacuumAgentPerceptStatusClean /* not in pseudocode */
   ,reflexVacuumAgentPerceptStatusDirty
  } status;
};

struct reflexVacuumAgentPercept *
reflexVacuumAgentPerceptNew(
  void
);

void
reflexVacuumAgentPerceptFree(
  struct reflexVacuumAgentPercept *
);

struct reflexVacuumAgentAction {
  enum {
    reflexVacuumAgentActionActSuck
   ,reflexVacuumAgentActionActMoveRight
   ,reflexVacuumAgentActionActMoveLeft
  } act;
};

struct reflexVacuumAgentAction *
reflexVacuumAgentActionNew(
  void
);

void
reflexVacuumAgentActionFree(
  struct reflexVacuumAgentAction *
);

/*
 * create an agent
 * return 0 on success
 * return 1 on invocation error
 * return 2 on resource error
 */

int
reflexVacuumAgent(
 chan_t *sensor
,chan_t *actuator
);
