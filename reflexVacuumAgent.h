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

/* put malloc'd instances of this on the percept channel */
struct reflexVacuumAgentPercept {
  enum {
    reflexVacuumAgentPerceptLocationA
   ,reflexVacuumAgentPerceptLocationB
  } location;
  enum {
    reflexVacuumAgentPerceptStatusDirty
  } status;
};

/* get malloc'd instances of this on the action channel */
struct reflexVacuumAgentAction {
  enum {
    reflexVacuumAgentActionActionSuck
   ,reflexVacuumAgentActionActionRight
   ,reflexVacuumAgentActionActionLeft
  } action;
};

/* agent implementation */
void
reflexVacuumAgent(
  struct agent *context
);
