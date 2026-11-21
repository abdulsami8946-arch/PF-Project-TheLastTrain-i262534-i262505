#include "level.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
const char* getEntityName(char entitySymbol)
{
    switch(entitySymbol)
    {
        case ENT_MASTER_KEY:
            return "Master Key";

        case ENT_ENERGY_PACK:
            return "Energy Pack";

        case ENT_EMERGENCY_TOOLKIT:
            return "Emergency Toolkit";

        case ENT_CLUE_4:
            return "Clue Digit 4";

        case ENT_FUSE:
            return "Fuse";

        case ENT_CAT:
            return "Stranded Cat";

        case ENT_CONTROL_PANEL:
            return "Control Panel";

        case ENT_LIFT_PASS:
            return "Lift Pass";

        case ENT_PASSENGER:
            return "Stranded Passenger";

        case ENT_MEDICAL_KIT:
            return "Medical Kit";

        case ENT_EMERGENCY_CARD:
            return "Emergency Access Card";

        case ENT_CLUE_2:
            return "Clue Digit 2";

        case ENT_CLUE_7:
            return "Clue Digit 7";

        case ENT_CABINET_TRAIN_KEY:
            return "Cabinet / Train Key";

        case ENT_CAT_CARRIER:
            return "Cat Carrier";

        case ENT_OBSERVATION_TOKEN:
            return "Observation Token";

        default:
            return "Unknown Entity";
    }
}

bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
ifstream file(filename);
if(!file)
    return false;
char mapLines[16][MAX_LINE_LEN];
for(int i = 0; i < 16; i++)
{
    if(!file.getline(mapLines[i], MAX_LINE_LEN))
        return false;
}
file.close();
const int labelRows[NUM_FLOORS] = {14, 11, 8, 5, 2};
const int entityRows[NUM_FLOORS] = {15, 12, 9, 6, 3};
const int connectionCols[ROOMS_PER_FLOOR - 1] = {20, 32, 44};
for(int f = 0; f < NUM_FLOORS; f++)
{
    if(strlen(mapLines[labelRows[f]]) < 57 ||
       strlen(mapLines[entityRows[f]]) < 57)
        return false;
    for(int r = 0; r < ROOMS_PER_FLOOR; r++)
    {
        entityCounts[f][r] = 0;
        for(int k = 0; k < MAX_ENTITIES_PER_ROOM; k++)
            roomEntities[f][r][k] = ENT_NONE;
    }
    for(int c = 0; c < ROOMS_PER_FLOOR - 1; c++)
    {
        char connection = mapLines[labelRows[f]][connectionCols[c]];
        if(connection == CONN_DOOR ||
           connection == CONN_WINDOW ||
           connection == CONN_LOCKED_DOOR ||
           connection == CONN_SOLID_WALL)
        {
            horizontalConn[f][c] = connection;
        }
        else
        {
            horizontalConn[f][c] = CONN_NONE;
        }
    }
    for(int r = 0; r < ROOMS_PER_FLOOR; r++)
    {
        int start = 10 + r * 12;
        int end = start + 11;
        for(int col = start; col < end; col++)
        {
            char symbol = mapLines[entityRows[f]][col];
            bool validEntity = false;
            switch(symbol)
            {
                case ENT_MASTER_KEY:
                case ENT_ENERGY_PACK:
                case ENT_EMERGENCY_TOOLKIT:
                case ENT_CLUE_4:
                case ENT_FUSE:
                case ENT_CAT:
                case ENT_CONTROL_PANEL:
                case ENT_LIFT_PASS:
                case ENT_PASSENGER:
                case ENT_MEDICAL_KIT:
                case ENT_EMERGENCY_CARD:
                case ENT_CLUE_2:
                case ENT_CLUE_7:
                case ENT_CABINET_TRAIN_KEY:
                case ENT_CAT_CARRIER:
                case ENT_OBSERVATION_TOKEN:
                    validEntity = true;
                    break;
            }
            if(validEntity)
            {
                int count = entityCounts[f][r];
                if(count >= MAX_ENTITIES_PER_ROOM)
                    return false;
                roomEntities[f][r][count] = symbol;
                entityCounts[f][r]++;
            }
        }
    }
}
    return true;
}


// Modular implementation using primitive arrays for map parsing and verification
