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
bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft)
{
    ifstream file(filename);

    if(!file)
        return false;

    playerFloor = 0;
    playerRoom = 0;

    energy = INITIAL_ENERGY;
    time = INITIAL_TIME;
    score = INITIAL_SCORE;

    inventoryCount = 0;
    emergencyTripsLeft = 0;

    for(int i = 0; i < MAX_INVENTORY_CAPACITY; i++)
        inventory[i] = ENT_NONE;

    for(int i = 0; i < NUM_OBJECTIVES; i++)
        objectives[i] = false;

    char line[MAX_LINE_LEN];

    while(file.getline(line, MAX_LINE_LEN))
    {
        char* colon = strchr(line, ':');

        if(colon == NULL)
            continue;

        *colon = '\0';

        char* value = colon + 1;

        while(*value == ' ' || *value == '\t')
            value++;

        int len = strlen(value);

        while(len > 0 && (value[len - 1] == ' ' || value[len - 1] == '\r'))
        {
            value[len - 1] = '\0';
            len--;
        }

        if(strcmp(line, "INITIAL_ENERGY") == 0)
        {
            energy = 0;

            for(int i = 0; value[i] >= '0' && value[i] <= '9'; i++)
                energy = energy * 10 + (value[i] - '0');
        }

        else if(strcmp(line, "INITIAL_TIME") == 0)
        {
            time = 0;

            for(int i = 0; value[i] >= '0' && value[i] <= '9'; i++)
                time = time * 10 + (value[i] - '0');
        }

        else if(strcmp(line, "INITIAL_SCORE") == 0)
        {
            score = 0;

            for(int i = 0; value[i] >= '0' && value[i] <= '9'; i++)
                score = score * 10 + (value[i] - '0');
        }

        else if(strcmp(line, "INITIAL_INVENTORY") == 0)
        {
            if(strcmp(value, "NONE") == 0)
                inventoryCount = 0;
        }

        else if(strcmp(line, "PASSENGER_RESCUED") == 0)
            objectives[OBJ_PASSENGER_RESCUED] = (strcmp(value, "YES") == 0);

        else if(strcmp(line, "CAT_RESCUED") == 0)
            objectives[OBJ_CAT_SECURED] = (strcmp(value, "YES") == 0);

        else if(strcmp(line, "CLUES_COLLECTED") == 0)
        {
            if(strcmp(value, "NONE") == 0)
            {
                objectives[OBJ_CLUE_4] = false;
                objectives[OBJ_CLUE_2] = false;
                objectives[OBJ_CLUE_7] = false;
            }
        }

        else if(strcmp(line, "CABINET_UNLOCKED") == 0)
            objectives[OBJ_CABINET_UNLOCKED] = (strcmp(value, "YES") == 0);

        else if(strcmp(line, "TRAIN_KEY_COLLECTED") == 0)
            objectives[OBJ_TRAIN_KEY] = (strcmp(value, "YES") == 0);

        else if(strcmp(line, "FUSE_INSTALLED") == 0)
            objectives[OBJ_FUSE_INSTALLED] = (strcmp(value, "YES") == 0);

        else if(strcmp(line, "LIFT_PASS_COLLECTED") == 0)
            objectives[OBJ_LIFT_PASS] = (strcmp(value, "YES") == 0);

        else if(strcmp(line, "TRAIN_PREPARED") == 0)
            objectives[OBJ_TRAIN_PREPARED] = (strcmp(value, "YES") == 0);
    }

    file.close();

    return true;
}
bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
{
    if(startFloor < 1 || startFloor > NUM_FLOORS ||
       startRoom < 1 || startRoom > ROOMS_PER_FLOOR)
    {
        return false;
    }

    int f = startFloor - 1;
    bool hasConnection = false;

    if(startRoom > 1)
    {
        char left = horizontalConn[f][startRoom - 2];

        if(left == CONN_DOOR ||
           left == CONN_WINDOW ||
           left == CONN_LOCKED_DOOR)
        {
            hasConnection = true;
        }
    }

    if(startRoom < ROOMS_PER_FLOOR)
    {
        char right = horizontalConn[f][startRoom - 1];

        if(right == CONN_DOOR ||
           right == CONN_WINDOW ||
           right == CONN_LOCKED_DOOR)
        {
            hasConnection = true;
        }
    }

    return hasConnection;
}


// Modular implementation using primitive arrays for map parsing and verification
