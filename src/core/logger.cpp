#include "logger.h"
#include "util.h"
#include <fstream>
using namespace std;

// File persistence for savegame.txt and game_summary.txt using primitive variables


bool saveGameStateToFile(const char* filename, int playerFloor, int playerRoom,
                         int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
    {
        ofstream file(filename);

        if (!file.is_open())
            {
                return false;
            }
        
        if (inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
            {
                return false;
            }

        file << playerFloor << endl;
        file << playerRoom << endl;
        file << energy << endl;
        file << time << endl;
        file << score << endl;
        file << inventoryCount << endl;
        file << emergencyTripsLeft << endl;

        for (int i = 0; i < inventoryCount; i++)
            {
                file << inventory[i] << endl;
            }

        for (int i = 0; i < NUM_OBJECTIVES; i++)
            {
                file << objectives[i] << endl;
            }

        for (int f = 0; f < NUM_FLOORS; f++)
            {
                for (int r = 0; r < ROOMS_PER_FLOOR; r++)
                    {
                        if (entityCounts[f][r] < 0 || entityCounts[f][r] > MAX_ENTITIES_PER_ROOM)
                            {
                                return false;
                            }
                        file << entityCounts[f][r] << endl;
                        for (int e = 0; e < entityCounts[f][r]; e++)
                            {
                                file << roomEntities[f][r][e] << endl;
                            }
                    }
            }

        file.close();
        return true;
    }

bool loadGameStateFromFile(const char* filename, int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
    {
        ifstream file(filename);

        if (!file.is_open())
            {
                return false;
            }

       if (!(file >> playerFloor))
            {
                return false;
            }
        if (!(file >> playerRoom))
            {
                return false;
            }
        if (!(file >> energy))
            {
                return false;
            }
        if (!(file >> time))
            {
                return false;
            }
        if (!(file >> score))
            {
                return false;
            }
        if (!(file >> inventoryCount))
            {
                return false;
            }
        if (!(file >> emergencyTripsLeft))
            {
                return false;
            }

        if (inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
            {
                return false;
            }

        for (int i = 0; i < inventoryCount; i++)
            {
                if (!(file >> inventory[i]))
                    {
                        return false;
                    }
            }

        for (int i = 0; i < NUM_OBJECTIVES; i++)
            {
                if (!(file >> objectives[i]))
                    {
                        return false;
                    }
            }

        for (int f = 0; f < NUM_FLOORS; f++)
            {
                for (int r = 0; r < ROOMS_PER_FLOOR; r++)
                    {
                        if (!(file >> entityCounts[f][r]))
                            {
                                return false;
                            }
                        if (entityCounts[f][r] < 0 || entityCounts[f][r] > MAX_ENTITIES_PER_ROOM)
                            {
                                return false;
                            }
                        for (int e = 0; e < entityCounts[f][r]; e++)
                            {
                                if (!(file >> roomEntities[f][r][e]))
                                    {
                                        return false;
                                    }
                            }
                    }
            }

        file.close();
        return true;
    }

void writeGameSummaryReport(const char* filename, int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory)
    {
        ofstream file(filename);

        if (!file.is_open())
            {
                return;
            }

        file << "Game Summary Report" << endl;
        file << "===================" << endl;
        file << "Final Score: " << score << endl;
        file << "Remaining Energy: " << remainingEnergy << endl;
        file << "Remaining Time: " << remainingTime << endl;
        file << "Mission Status: " << (victory ? "Victory" : "Defeat") << endl;

        file << "\nObjectives Status:" << endl;
        for (int i = 0; i < NUM_OBJECTIVES; i++)
            {
                file << "Objective " << i + 1 << ": " << (objectives[i] ? "Completed" : "Not Completed") << endl;
            }

        file.close();
    }