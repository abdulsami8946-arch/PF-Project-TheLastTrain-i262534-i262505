#include "simulator.h"
#include "level.h"
#include "logger.h"
#include "viewer_iface.h"
#include <iostream>
using namespace std;

int main() {
    // Player State Primitives
    int playerFloor = 1;
    int playerRoom = 1;
    int energy = INITIAL_ENERGY;
    int time = INITIAL_TIME;
    int score = INITIAL_SCORE;
    
    char inventory[MAX_INVENTORY_CAPACITY];
    int inventoryCount = 0;
    int emergencyTripsLeft = 0;

    // Objective State Booleans
    bool objectives[NUM_OBJECTIVES] = { false };

    // Building Connectivity & Entity Parallel Matrices
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR] = { 0 };

    const char* mapFile = "data/building.txt";
    const char* initStateFile = "data/initial-state.txt";

    if (!loadBuildingMap(mapFile, horizontalConn, roomEntities, entityCounts)) {
        std::cerr << "Error: Could not load " << mapFile << "\n";
        return 1;
    }

    if (!loadInitialState(initStateFile, playerFloor, playerRoom, energy, time, score,
                          inventory, inventoryCount, objectives, emergencyTripsLeft)) {
        std::cerr << "Error: Could not load " << initStateFile << "\n";
        return 1;
    }

    std::cout << "===========================================\n";
    std::cout << " THE LAST TRAIN - SIMULATION INITIALIZED   \n";
    std::cout << "===========================================\n";

    runSimulation(playerFloor, playerRoom, energy, time, score,
                  inventory, inventoryCount, objectives, emergencyTripsLeft,
                  horizontalConn, roomEntities, entityCounts);
cout<<"sohail";
    return 0;
}
