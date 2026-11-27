#include <iostream>
#include <string>
#include <cstring>

#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include "level.h"

using namespace std;

// Modular simulation loop using primitive variables

// ==================== SIMULATION LOOP ====================

void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
    {
        bool simulationRunning = true;

        while (simulationRunning)
        {
            if (playerFloor < 1 || playerFloor > NUM_FLOORS || playerRoom < 1 || playerRoom > ROOMS_PER_FLOOR)
                {
                    cout << "Invalid player location." << endl;
                    break;
                }

            renderDashboard(playerFloor, playerRoom, energy, time, score, inventory, inventoryCount, emergencyTripsLeft);
            renderRoomDetails(playerFloor, playerRoom, roomEntities, entityCounts[playerFloor - 1][playerRoom - 1]);
            renderAsciiBuilding(playerFloor, playerRoom);
            renderObjectivesChecklist(objectives);

            string command;

            cout << "\nAvailable commands:" << endl;
            cout << "move     - Move to another room" << endl;
            cout << "inspect  - Inspect current room" << endl;
            cout << "collect  - Collect an item" << endl;
            cout << "use      - Use an inventory item" << endl;
            cout << "unlock   - Unlock the security cabinet" << endl;
            cout << "save     - Save game" << endl;
            cout << "load     - Load saved game" << endl;
            cout << "exit     - Exit simulation" << endl;

            cout << "\nEnter your action: ";

            if (!(cin >> command))
                {
                    simulationRunning = false;
                    break;
                }

            if (command == "exit")
            {
                simulationRunning = false;
                cout << "Exiting simulation." << endl;
            }

            else if (command == "move")
            {
                int targetFloor, targetRoom;

                cout << "Enter target floor: ";
                cin >> targetFloor;

                cout << "Enter target room: ";
                cin >> targetRoom;

                if (handleMoveCommand(targetFloor, targetRoom, playerFloor, playerRoom, energy, time, horizontalConn,
                                  inventory, inventoryCount, emergencyTripsLeft, objectives[OBJ_LIFT2_ACTIVE]))
                    {
                        cout << "Movement successful!" << endl;
                    }
                else
                {
                    cout << "Movement failed! Check connectivity or access requirements." << endl;
                }
            }

            else if (command == "inspect")
            {
                int f = playerFloor - 1;
                int r = playerRoom - 1;

                if (handleInspectCommand(playerFloor, playerRoom, energy, time, roomEntities, entityCounts[f][r], objectives, score))
                    {
                        cout << "Room inspected successfully!" << endl;

                    for (int i = 0; i < entityCounts[f][r]; i++)
                        {
                            char symbol = roomEntities[f][r][i];
                            if (symbol == ENT_CLUE_4 && !objectives[OBJ_CLUE_4])
                                {
                                    objectives[OBJ_CLUE_4] = true;
                                    applyScoreDelta(score, 10);
                                    cout << "Clue digit 4 discovered!" << endl;
                                }
                            else if (symbol == ENT_CLUE_2 && !objectives[OBJ_CLUE_2])
                                {
                                    objectives[OBJ_CLUE_2] = true;
                                    applyScoreDelta(score, 10);
                                    cout << "Clue digit 2 discovered!" << endl;
                                }
                            else if (symbol == ENT_CLUE_7 && !objectives[OBJ_CLUE_7])
                                {
                                    objectives[OBJ_CLUE_7] = true;
                                    applyScoreDelta(score, 10);
                                    cout << "Clue digit 7 discovered!" << endl;
                                }
                        }
                    }
                else
                    {
                        cout << "Inspection failed! Check energy, time, or room state." << endl;
                    }
            }

            else if (command == "collect")
            {
                char itemSymbol;

                cout << "Enter item symbol to collect: ";
                cin >> itemSymbol;

                int f = playerFloor - 1;
                int r = playerRoom - 1;

                if (handleCollectCommand(itemSymbol, playerFloor, playerRoom, energy, time, score, roomEntities, entityCounts[f][r], inventory, inventoryCount, objectives))
                    {
                        cout << "Item collected successfully!" << endl;

                        if (itemSymbol == ENT_EMERGENCY_CARD)
                            {
                                emergencyTripsLeft = 3;
                                cout << "Three emergency lift trips are available." << endl;
                            }
                    }
                else
                    {
                        cout << "Could not collect item!" << endl;
                    }
                }

            else if (command == "use")
                {
                    char itemSymbol;
                    cout << "Enter item symbol to use: ";
                    cin >> itemSymbol;

                    if (handleUseCommand(itemSymbol, playerFloor, playerRoom, energy, time, score, inventory, inventoryCount, objectives))
                        {
                            cout << "Item used successfully!" << endl;
                        }
                    else
                        {
                            cout << "Could not use item! Check its requirements and location." << endl;
                        }
                }

            else if (command == "unlock")
                {
                    char code[4] = "";
                    cout << "Enter cabinet code: ";

                    cin.width(4);
                    cin >> code;
            
                    int f = playerFloor - 1;
                    int r = playerRoom - 1;

                    if (handleUnlockCabinetCommand(code, playerFloor, playerRoom, objectives, score, roomEntities, entityCounts[f][r]))
                        {
                            cout << "Cabinet unlocked successfully!" << endl;
                            cout << "Train Key acquired!" << endl;
                        }
                    else
                        {
                            cout << "Could not unlock cabinet!" << endl;
                        }
                }

            else if (command == "save")
                {
                    if (saveGameStateToFile("./out/savegame.txt", playerFloor, playerRoom, energy, time, score, inventory, 
                        inventoryCount, objectives, emergencyTripsLeft, roomEntities, entityCounts))
                        {
                            cout << "Game saved successfully!" << endl;
                        }
                    else
                        {
                            cout << "Could not save game!" << endl;
                        }
                }

            else if (command == "load")
                {
                    if (loadGameStateFromFile("./out/savegame.txt", playerFloor, playerRoom, energy, time, score, inventory, inventoryCount,
                        objectives, emergencyTripsLeft, roomEntities, entityCounts))
                        {
                            cout << "Game loaded successfully!" << endl;
                        }
                    else
                        {
                            cout << "Could not load game!" << endl;
                        }
                }

            else
                {
                    cout << "Unknown command. Try again." << endl;
                }

        // Check whether the mission has ended.

            if (!checkEndConditions(energy, time, objectives))
                {
                    simulationRunning = false;
                    if (!objectives[OBJ_MISSION_WON])
                        {
                            objectives[OBJ_MISSION_FAILED] = true;
                        }
                }
        }
        writeGameSummaryReport("./out/game_summary.txt", score, energy, time, objectives, objectives[OBJ_MISSION_WON]);
    }


// MOVEMENT COMMAND 

bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY],
                       int inventoryCount,
                       int& emergencyTripsLeft,
                       bool lift2Operational)
    {
        if (targetFloor < 1 || targetFloor > NUM_FLOORS || targetRoom < 1 || targetRoom > ROOMS_PER_FLOOR)
            {
                return false;
            }

        if (currentFloor < 1 || currentFloor > NUM_FLOORS || currentRoom < 1 || currentRoom > ROOMS_PER_FLOOR)
            {
                return false;
            }

        if (targetFloor == currentFloor && targetRoom == currentRoom)
            {
                return false;
            }

        char connection = CONN_NONE;

        int deltaEnergy = -2;
        int deltaTime = -1;

        bool validMove = false;
        bool emergencyLiftTrip = false;

        // Check horizontal room connections.

        if (targetFloor == currentFloor)
            {
                if (isDirectlyConnected(currentFloor, currentRoom, targetFloor, targetRoom, horizontalConn, connection))
                    {
                        if (connection == CONN_DOOR || connection == CONN_WINDOW || connection == CONN_LOCKED_DOOR)
                            {
                                if (canTraverse(targetFloor, targetRoom, connection, inventory, inventoryCount, emergencyTripsLeft, lift2Operational))
                                    {
                                        validMove = true;
                                        if (connection == CONN_WINDOW)
                                            {
                                                deltaEnergy = -3;
                                                deltaTime = -2;
                                            }
                                    }
                            }
                    }
            }

    // Check staircases.

        else
            {
                if ((currentFloor == 1 && currentRoom == 1 && targetFloor == 2 && targetRoom == 1) ||
                    (currentFloor == 2 && currentRoom == 1 && targetFloor == 1 && targetRoom == 1) ||
                    (currentFloor == 2 && currentRoom == 1 && targetFloor == 3 && targetRoom == 1) ||
                    (currentFloor == 3 && currentRoom == 1 && targetFloor == 2 && targetRoom == 1) ||
                    (currentFloor == 4 && currentRoom == 1 && targetFloor == 5 && targetRoom == 1) ||
                    (currentFloor == 5 && currentRoom == 1 && targetFloor == 4 && targetRoom == 1))
                        {
                            validMove = true;
                        }
// Check Lift 1.
                else if (canUseLift1(currentFloor, targetFloor) && currentRoom == targetRoom && (currentRoom == 2 || currentRoom == 3))
                        {
                            validMove = true;
                            deltaEnergy = -4;
                            deltaTime = -3;
                        }
// Check Lift 2.
                else if (currentRoom == 4 && targetRoom == 4 && ((currentFloor == 2 || currentFloor == 4 || currentFloor == 5) &&
                        (targetFloor == 2 || targetFloor == 4 || targetFloor == 5)))
                            {
                                if (lift2Operational)
                                {
                                    if (hasItem(inventory, inventoryCount, ENT_LIFT_PASS))
                                    {
                                        validMove = true;
                                    }
                                }
                                else if (emergencyTripsLeft > 0 && hasItem(inventory, inventoryCount, ENT_EMERGENCY_CARD))
                                    {
                                        validMove = true;
                                        emergencyLiftTrip = true;
                                    }

                                if (validMove)
                                    {
                                        deltaEnergy = -4;
                                        deltaTime = -3;
                                    }
                            }
            }

        if (!validMove)
            {
                return false;
            }
// Apply costs only after a valid movement.

        applyResourceDelta(energy, time, deltaEnergy, deltaTime);

        if (emergencyLiftTrip)
            {
                emergencyTripsLeft--;
            }

        currentFloor = targetFloor;
        currentRoom = targetRoom;

        return true;
    }


// INSPECT COMMAND 

bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES],
                          int& score)
    {
        if (currentFloor < 1 || currentFloor > NUM_FLOORS || currentRoom < 1 || currentRoom > ROOMS_PER_FLOOR)
            {
                return false;
            }
        if (energy <= 0 || time <= 0 || entityCount < 0 || entityCount > MAX_ENTITIES_PER_ROOM)
            {
                return false;
            }
        applyResourceDelta(energy, time, -1, -1);
        cout << "\nRoom inspection:" << endl;
        if (entityCount == 0)
            {
                cout << "No entities found in this room." << endl;
            }
        else
            {
                for (int i = 0; i < entityCount; i++)
                    {
                        cout << getEntityName(roomEntities[currentFloor - 1] [currentRoom - 1][i]) << endl;
                    }
            }
        return true;
    }


//  COLLECT COMMAND 

bool handleCollectCommand(char itemSymbol,
                          int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY],
                          int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES])
    {
        if (currentFloor < 1 || currentFloor > NUM_FLOORS || currentRoom < 1 || currentRoom > ROOMS_PER_FLOOR)
            {
                return false;
            }
        if (energy <= 0 || time <= 0 || entityCount < 0 || entityCount > MAX_ENTITIES_PER_ROOM || inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
            {
                return false;
            }
        int position = -1;
// Find the requested entity in the current room.
        for (int i = 0; i < entityCount; i++)
            {
                if (roomEntities[currentFloor - 1][currentRoom - 1][i] == itemSymbol)
                    {
                        position = i;
                        break;
                    }
            }

        if (position == -1)
            {
                return false;
            }
// These entities have special interactions.
        if (itemSymbol == ENT_CAT || itemSymbol == ENT_PASSENGER || itemSymbol == ENT_CONTROL_PANEL || itemSymbol == ENT_CABINET_TRAIN_KEY)
            {
                return false;
            }
// Clue digits must first be discovered through inspection.
        if (itemSymbol == ENT_CLUE_4 && !objectives[OBJ_CLUE_4])
            {
                return false;
            }
        if (itemSymbol == ENT_CLUE_2 && !objectives[OBJ_CLUE_2])
            {
                return false;
            }
        if (itemSymbol == ENT_CLUE_7 && !objectives[OBJ_CLUE_7])
            {
                return false;
            }
// Prevent duplicate collection of important items.
        if (itemSymbol == ENT_MASTER_KEY && objectives[OBJ_MASTER_KEY])
            {
                return false;
            }
        if (itemSymbol == ENT_FUSE && objectives[OBJ_FUSE_COLLECTED])
            {
                return false;
            }
        if (itemSymbol == ENT_LIFT_PASS && objectives[OBJ_LIFT_PASS])
            {
                return false;
            }
        if (itemSymbol == ENT_CAT_CARRIER && objectives[OBJ_CAT_CARRIER])
            {
                return false;
            }
        if (itemSymbol == ENT_MEDICAL_KIT && hasItem(inventory, inventoryCount, ENT_MEDICAL_KIT))
            {
                return false;
            }
        if (itemSymbol == ENT_EMERGENCY_CARD && hasItem(inventory, inventoryCount, ENT_EMERGENCY_CARD))
            {
                return false;
            }
// Energy packs and special resources do not occupy inventory.
        bool automaticItem = itemSymbol == ENT_ENERGY_PACK || itemSymbol == ENT_EMERGENCY_TOOLKIT || itemSymbol == ENT_OBSERVATION_TOKEN;
// Clues are already recorded as objective flags.
        bool clueItem = itemSymbol == ENT_CLUE_4 || itemSymbol == ENT_CLUE_2 || itemSymbol == ENT_CLUE_7;
        if (!automaticItem && !clueItem)
            {
                if (inventoryCount >= MAX_INVENTORY_CAPACITY)
                    {
                        return false;
                    }
            if (!addItem(inventory, inventoryCount, itemSymbol))
                    {
                        return false;
                    }
            }
// Pay the normal pickup cost.
        applyResourceDelta(energy, time, -1, -1);
// Apply special resource bonuses.
        if (itemSymbol == ENT_ENERGY_PACK)
            {
                applyResourceDelta(energy, time, 25, 0);
            }
        else if (itemSymbol == ENT_EMERGENCY_TOOLKIT)
            {
                applyResourceDelta(energy, time, 15, 0);
            }
        else if (itemSymbol == ENT_OBSERVATION_TOKEN)
            {
                applyResourceDelta(energy, time, 0, 15);
            }
// Update collection objectives and score.
        if (itemSymbol == ENT_MASTER_KEY)
            {
                objectives[OBJ_MASTER_KEY] = true;
                applyScoreDelta(score, 10);
            }
        else if (itemSymbol == ENT_FUSE)
            {
                objectives[OBJ_FUSE_COLLECTED] = true;
            }
        else if (itemSymbol == ENT_LIFT_PASS)
            {
                objectives[OBJ_LIFT_PASS] = true;
                applyScoreDelta(score, 20);
            }
        else if (itemSymbol == ENT_CAT_CARRIER)
            {
                objectives[OBJ_CAT_CARRIER] = true;
            }
// Remove the collected entity from the room.
        for (int i = position; i < entityCount - 1; i++)
            {
                roomEntities[currentFloor - 1][currentRoom - 1][i] = roomEntities[currentFloor - 1][currentRoom - 1][i + 1];
            }

        entityCount--;
        roomEntities[currentFloor - 1][currentRoom - 1][entityCount] = ENT_NONE;
        return true;
    }


//  USE COMMAND 

bool handleUseCommand(char itemSymbol,
                      int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY],
                      int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES])
    {
        if (currentFloor < 1 || currentFloor > NUM_FLOORS || currentRoom < 1 || currentRoom > ROOMS_PER_FLOOR)
            {
                return false;
            }
        if (energy <= 0 || time <= 0 || inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
            {
                return false;
            }
        if (!hasItem(inventory, inventoryCount, itemSymbol))
            {
                return false;
            }
// Install the Fuse at the Control Panel.

        if (itemSymbol == ENT_FUSE)
            {
                if (currentFloor != 3 || currentRoom != 4 || objectives[OBJ_FUSE_INSTALLED])
                    {
                        return false;
                    }   
                if (!removeItem(inventory, inventoryCount, ENT_FUSE))
                    {
                        return false;
                    }

                objectives[OBJ_FUSE_INSTALLED] = true;
                objectives[OBJ_LIFT2_ACTIVE] = true;
                applyResourceDelta(energy, time, -1, -1);
                applyScoreDelta(score, 20);

                cout << "Fuse installed. Lift 2 is now operational!" << endl;

                return true;
            }
// Secure the Cat using the Cat Carrier.
        if (itemSymbol == ENT_CAT_CARRIER)
            {
                if (currentFloor != 3 || currentRoom != 3 || objectives[OBJ_CAT_SECURED])
                    {
                        return false;
                    }
                if (!removeItem(inventory, inventoryCount, ENT_CAT_CARRIER))
                    {
                        return false;
                    }

                objectives[OBJ_CAT_SECURED] = true;

                applyResourceDelta(energy, time, -1, -1);
                applyScoreDelta(score, 100);

                cout << "The stranded cat has been secured!" << endl;

                return true;
            }
// Rescue the injured passenger.

        if (itemSymbol == ENT_MEDICAL_KIT)
            {
                if (currentFloor != 4 || currentRoom != 1 || objectives[OBJ_PASSENGER_RESCUED])
                    {
                        return false;
                    }
                if (!removeItem(inventory, inventoryCount, ENT_MEDICAL_KIT))
                    {
                        return false;
                    }
                objectives[OBJ_PASSENGER_RESCUED] = true;

                applyResourceDelta(energy, time, -1, -1);
                applyScoreDelta(score, 100);

                cout << "The injured passenger has been rescued!" << endl;

                return true;
            }
// Prepare and board the final train.
        if (itemSymbol == ENT_CABINET_TRAIN_KEY)
            {
                if (currentFloor != 5 || currentRoom != 2 || !objectives[OBJ_TRAIN_KEY] ||
                !objectives[OBJ_PASSENGER_RESCUED] || !objectives[OBJ_CAT_SECURED] || objectives[OBJ_TRAIN_PREPARED])
                    {
                        return false;
                    }
                applyResourceDelta(energy, time, -1, -1);
                objectives[OBJ_TRAIN_PREPARED] = true;
                applyScoreDelta(score, 100);
// Full mission victory bonus.
                objectives[OBJ_MISSION_WON] = true;
                applyScoreDelta(score, 100);
                cout << "Train prepared and boarded!" << endl;
                cout << "MISSION COMPLETE!" << endl;
                return true;
            }
        return false;
    }

// CABINET UNLOCK COMMAND 

bool handleUnlockCabinetCommand(const char* code,
                                int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES],
                                int& score,
                                char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                                int& entityCount)
    {
        if (currentFloor != 5 || currentRoom != 1)
            {
                return false;
            }
        if (objectives[OBJ_CABINET_UNLOCKED] || entityCount < 0 || entityCount > MAX_ENTITIES_PER_ROOM)
            {
                return false;
            }
        if (code == NULL)
            {
                return false;
            }
        if (!verifyCabinetCode(code, objectives))
            {
                return false;
            }
        int position = -1;
// Find the cabinet in the current room.
        for (int i = 0; i < entityCount; i++)
            {
                if (roomEntities[currentFloor - 1][currentRoom - 1][i] == ENT_CABINET_TRAIN_KEY)
                    {
                        position = i;
                        break;
                    }
            }
        if (position == -1)
            {
                return false;
            }
        objectives[OBJ_CABINET_UNLOCKED] = true;
        objectives[OBJ_TRAIN_KEY] = true;
        applyScoreDelta(score, 30);
        applyScoreDelta(score, 10);
    
// Remove the cabinet entity after unlocking.

        for (int i = position; i < entityCount - 1; i++)
            {
                roomEntities[currentFloor - 1][currentRoom - 1][i] = roomEntities[currentFloor - 1][currentRoom - 1][i + 1];
            }

        entityCount--;
        roomEntities[currentFloor - 1][currentRoom - 1][entityCount] = ENT_NONE;
        return true;
    }


// END CONDITIONS

bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES])
    {
        if (energy <= 0 || time <= 0)
            {
                cout << "\nMISSION FAILED!" << endl;
                cout << "Energy or time has run out." << endl;
                return false;
            }
        if (objectives[OBJ_MISSION_WON])
            {
                cout << "\nCONGRATULATIONS!" << endl;
                cout << "You successfully completed THE LAST TRAIN!" << endl;
                return false;
            }
        return true;
    }
