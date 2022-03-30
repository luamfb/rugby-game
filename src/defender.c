// Standard headers
#include <stdio.h>
#include <stdlib.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "defender.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

// extern functions
// implemented in attacker.c
extern void initialize_random_seed_if_needed(void);

/*----------------------------------------------------------------------------*/
/*                             PRIVATE FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

static direction_t defender_choose_direction(void) {
  int random_value = rand() % 20;
  if (random_value == 0) {
    return (direction_t) DIR_UP;
  }
  if (random_value == 1) {
    return (direction_t) DIR_DOWN;
  }
  return (direction_t) DIR_STAY;
}

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_defender_strategy(
    position_t defender_position, Spy attacker_spy) {

  // I don't even know the grid size, so I won't bother with spying on
  // the attacker either
  UNUSED(attacker_spy);

  // This defender is mostly stationary, so even if it hit an obstacle and
  // couldn't move last turn, that's OK
  UNUSED(defender_position);

  initialize_random_seed_if_needed();

  return defender_choose_direction();
}

/*----------------------------------------------------------------------------*/
