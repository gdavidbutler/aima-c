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

/* sensor - what an agent uses to form percepts of an environment */
typedef struct sensor {
  void *opaque;
} sensor_t;

/* percept - what an agent perceives, through sensors, of an environment */
typedef struct percept {
  void *opaque;
} percept_t;

/* perceptSequence - a sequence of percepts that result in an action */
typedef struct perceptSequence {
  percept_t *percept;
  unsigned int perceptCount;
} perceptSequence_t;

/* actuator - an agent uses actuators to act within an environment */
typedef struct actuator {
  void *opaque;
} actuator_t;

/* an agent perceives and acts within an environment */
typedef struct agent {
  void *opaque;
  sensor_t sensor;
  actuator_t actuator;
  perceptSequence_t perceptSequence;
  void (*function)(struct agent *self);
} agent_t;

agent_t *newAgent();
void deleteAgent(agent_t *self);
