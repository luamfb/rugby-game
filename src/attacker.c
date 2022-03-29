// Standard headers
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "attacker.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter
#define DIR_INVALID { INT_MAX, INT_MAX }

// Enums
/*
 * These indices are meant to represent each possible direction in which
 * the attacker can move, except DIR_KEEP and DIR_INVALID.
 * Lower values should correspond to directions more likely to be chosen.
 */
typedef enum {
  DIR_INDEX_RIGHT,
  DIR_INDEX_UP_RIGHT,
  DIR_INDEX_DOWN_RIGHT,
  DIR_INDEX_UP,
  DIR_INDEX_DOWN,
  DIR_INDEX_UP_LEFT,
  DIR_INDEX_DOWN_LEFT,
  DIR_INDEX_LEFT,
} dir_index_t;

#define DIR_INDEX_MAX (int)DIR_INDEX_LEFT + 1

/*----------------------------------------------------------------------------*/
/*                             PRIVATE FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

static inline void clear_direction_blacklist(
    bool *blacklisted_directions,
    size_t direction_count) {
  memset(blacklisted_directions, 0, direction_count * sizeof(bool));
}

static inline int equal_directions(direction_t dir1, direction_t dir2) {
  return memcmp(&dir1, &dir2, sizeof(direction_t));
}

static dir_index_t index_from_direction(direction_t direction) {
  if (equal_directions(direction, (direction_t) DIR_RIGHT)) {
    return DIR_INDEX_RIGHT;
  }
  if (equal_directions(direction, (direction_t) DIR_UP_RIGHT)) {
    return DIR_INDEX_UP_RIGHT;
  }
  if (equal_directions(direction, (direction_t) DIR_DOWN_RIGHT)) {
    return DIR_INDEX_DOWN_RIGHT;
  }

  if (equal_directions(direction, (direction_t) DIR_UP)) {
    return DIR_INDEX_UP;
  }
  if (equal_directions(direction, (direction_t) DIR_DOWN)) {
    return DIR_INDEX_DOWN;
  }

  if (equal_directions(direction, (direction_t) DIR_UP_LEFT)) {
    return DIR_INDEX_UP_LEFT;
  }
  if (equal_directions(direction, (direction_t) DIR_DOWN_LEFT)) {
    return DIR_INDEX_DOWN_LEFT;
  }
  return DIR_INDEX_LEFT;
}

static inline direction_t direction_from_index(dir_index_t direction_index) {
  switch (direction_index) {
  case DIR_INDEX_RIGHT:
    return (direction_t) DIR_RIGHT;
  case DIR_INDEX_UP_RIGHT:
    return (direction_t) DIR_UP_RIGHT;
  case DIR_INDEX_DOWN_RIGHT:
    return (direction_t) DIR_DOWN_RIGHT;
  case DIR_INDEX_UP:
    return (direction_t) DIR_UP;
  case DIR_INDEX_DOWN:
    return (direction_t) DIR_DOWN;
  case DIR_INDEX_UP_LEFT:
    return (direction_t) DIR_UP_LEFT;
  case DIR_INDEX_DOWN_LEFT:
    return (direction_t) DIR_DOWN_LEFT;
  case DIR_INDEX_LEFT:
  default:
    return (direction_t) DIR_LEFT;
  }
}

static direction_t attacker_choose_direction(
    bool *direction_blacklist, size_t direction_count) {
  int last_index = direction_count - 1;
  for (int i = 0; i < last_index; i++) {
    if (direction_blacklist[i]) {
      continue;
    }
    if (rand() % 2) {
      return direction_from_index(i);
    }
  }
  // if we're here, no other direction was chosen
  return direction_blacklist[last_index] ?
    (direction_t)DIR_STAY : direction_from_index(last_index);
}

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

void initialize_random_seed_if_needed(void) {
  static bool initialized = false;
  if (!initialized) {
    srand(time(NULL));
    initialized = true;
  }
}

direction_t execute_attacker_strategy(
    position_t attacker_position, Spy defender_spy) {

  direction_t chosen_direction;
  static position_t previous_position = INVALID_POSITION;
  static direction_t previous_direction = DIR_INVALID;
  static bool blacklisted_directions[DIR_INDEX_MAX];

  // I don't even know the grid size, so I won't bother with spying on
  // the defender either
  UNUSED(defender_spy);

  initialize_random_seed_if_needed();

  if (equal_positions(attacker_position, previous_position)
      && !equal_directions(previous_direction, (direction_t) DIR_INVALID)) {
    int direction_index = index_from_direction(previous_direction);
    blacklisted_directions[direction_index] = true;
  } else {
    // In the first call to this function,
    // previous_direction is equal to DIR_INVALID and thus we fall here,
    // which initializes blacklisted_directions before its first use.
    clear_direction_blacklist(blacklisted_directions, DIR_INDEX_MAX);
  }

  chosen_direction = attacker_choose_direction(
    blacklisted_directions,
    DIR_INDEX_MAX);

  previous_position = attacker_position;
  previous_direction = chosen_direction;

  return chosen_direction;
}

/*----------------------------------------------------------------------------*/
