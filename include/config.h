#ifndef CONFIG_H
#define CONFIG_H

// --- Facility & Grid Constants ---
const int NUM_FLOORS = 5;
const int ROOMS_PER_FLOOR = 4;
const int TOTAL_ROOMS = 20;

// --- Resource Pools ---
const int MAX_ENERGY = 100;
const int MAX_TIME = 100;
const int INITIAL_ENERGY = 100;
const int INITIAL_TIME = 50;
const int INITIAL_SCORE = 0;

// --- Inventory & Array Capacities ---
const int MAX_INVENTORY_CAPACITY = 10;
const int MAX_ENTITIES_PER_ROOM = 5;
const int MAX_NAME_LEN = 32;
const int MAX_LINE_LEN = 256;

// --- Resource Action Costs ---
const int COST_MOVE_ORDINARY_ENERGY = -2;
const int COST_MOVE_ORDINARY_TIME   = -1;

const int COST_MOVE_WINDOW_ENERGY   = -3;
const int COST_MOVE_WINDOW_TIME     = -2;

const int COST_MOVE_LIFT_ENERGY     = -4;
const int COST_MOVE_LIFT_TIME       = -3;

const int COST_INSPECT_ENERGY       = -1;
const int COST_INSPECT_TIME         = -1;

const int COST_ITEM_PICKUP_ENERGY   = -1;
const int COST_ITEM_PICKUP_TIME     = -1;

const int COST_ITEM_USE_ENERGY      = -1;
const int COST_ITEM_USE_TIME        = -1;

// --- Resource Item Bonuses ---
const int BONUS_ENERGY_PACK         = 25;
const int BONUS_EMERGENCY_TOOLKIT   = 15;
const int BONUS_OBSERVATION_TOKEN   = 15;

// --- Score Awards ---
const int SCORE_KEY_COLLECT         = 10;
const int SCORE_CLUE_FOUND          = 10;
const int SCORE_CABINET_UNLOCK      = 30;
const int SCORE_TRAIN_KEY           = 10;
const int SCORE_RESCUE_PASSENGER    = 100;
const int SCORE_SECURE_CAT          = 100;
const int SCORE_INSTALL_FUSE        = 20;
const int SCORE_LIFT_PASS           = 20;
const int SCORE_PREPARE_TRAIN       = 100;
const int SCORE_MISSION_VICTORY     = 100;

#endif // CONFIG_H
