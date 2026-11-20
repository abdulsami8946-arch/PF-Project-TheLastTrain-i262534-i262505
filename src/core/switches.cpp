#include <cstring>
#include "switches.h"
#include "util.h"

// Door lock mechanics, lifts, and cabinet verification without structures

bool canUseLift1(int fromFloor, int toFloor) 
    {
    // Lift 1 can be used between floors 1 and 2 only
    return (fromFloor == 1 && toFloor == 2) || (fromFloor == 2 && toFloor == 1);
    }

bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft) 
    {
    if (!lift2Operational && emergencyTripsLeft <= 0) 
        {
        return false;
        }
    return (fromFloor == 2 && toFloor == 4) || (fromFloor == 4 && toFloor == 2) || (fromFloor == 4 && toFloor == 5) ||
       (fromFloor == 5 && toFloor == 4) || (fromFloor == 2 && toFloor == 5) ||(fromFloor == 5 && toFloor == 2);
    }

bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount)
    {
    // Unlocking the floor 3 door requires a specific key in the inventory
    const char requiredKey = ENT_MASTER_KEY;
    for (int i = 0; i < inventoryCount; i++) 
        {
            if (inventory[i] == requiredKey)
                {
                return true;
                }
        }
    return false;
    }

bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                             bool objectives[NUM_OBJECTIVES], int& score)
    {
    const char requiredItem = ENT_FUSE;
    for (int i = 0; i < inventoryCount; i++)
        {
            if (inventory[i] == requiredItem)
                {
                // Remove the fuse from inventory
                for (int j = i; j < inventoryCount - 1; j++)
                    {
                    inventory[j] = inventory[j + 1];
                    }
                inventoryCount--;
                // Mark the objective as completed
                objectives[OBJ_FUSE_INSTALLED] = true; 
                score += SCORE_INSTALL_FUSE;
                return true;
                }
        }
    return false;
    }

bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES])
    {
    const char correctCode[] = "427";
    if (!objectives[OBJ_CLUE_4] || !objectives[OBJ_CLUE_2] || !objectives[OBJ_CLUE_7])
        {
        return false; 
        }
    return strcmp(enteredCode, correctCode) == 0;
    }
        