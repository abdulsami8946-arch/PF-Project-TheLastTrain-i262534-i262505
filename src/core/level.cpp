
#include "level.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


// Converts an entity symbol into its readable name.
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


// Reads the building map and extracts room connections and entities.
bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    // Open the building file.
    ifstream file(filename);

    if(!file)
        return false;


    // Store the first 16 lines of the building map.
    char mapLines[16][MAX_LINE_LEN];

    for(int i = 0; i < 16; i++)
    {
        if(!file.getline(mapLines[i], MAX_LINE_LEN))
            return false;
    }

    file.close();


    // These arrays identify where the required information is located.
    const int labelRows[NUM_FLOORS] = {14, 11, 8, 5, 2};

    const int entityRows[NUM_FLOORS] = {15, 12, 9, 6, 3};

    const int connectionCols[ROOMS_PER_FLOOR - 1] = {20, 32, 44};


    // Process all five floors.
    for(int f = 0; f < NUM_FLOORS; f++)
    {
        // Make sure both required lines are long enough.
        if(strlen(mapLines[labelRows[f]]) < 57)
            return false;

        if(strlen(mapLines[entityRows[f]]) < 57)
            return false;


        // Prepare entity storage for all four rooms on this floor.
        for(int r = 0; r < ROOMS_PER_FLOOR; r++)
        {
            // Each room starts with zero recorded entities.
            entityCounts[f][r] = 0;

            // Set all five entity storage positions to empty.
            for(int k = 0; k < MAX_ENTITIES_PER_ROOM; k++)
            {
                roomEntities[f][r][k] = ENT_NONE;
            }
        }


        // Extract the three horizontal connections on this floor.
        for(int c = 0; c < ROOMS_PER_FLOOR - 1; c++)
        {
            // Read the character at the position of the current connection.
            char connection = mapLines[labelRows[f]][connectionCols[c]];

            // Check whether the character represents a valid connection.
            if(connection == CONN_DOOR ||
               connection == CONN_WINDOW ||
               connection == CONN_LOCKED_DOOR ||
               connection == CONN_SOLID_WALL ||
               connection == '1')
            {
                // Store the connection in the corresponding array position.
                horizontalConn[f][c] = connection;
            }
            else
            {
                // Store an empty connection if the character is not recognized.
                horizontalConn[f][c] = CONN_NONE;
            }
        }


        // Extract entities from all four rooms on the current floor.
        for(int r = 0; r < ROOMS_PER_FLOOR; r++)
        {
            // Calculate the character positions belonging to this room.
            int start = 10 + r * 12;
            int end = start + 11;


            // Examine each character position inside the current room section.
            for(int col = start; col < end; col++)
            {
                // Read one character from the current floor's entity row.
                char symbol = mapLines[entityRows[f]][col];

                // Initially assume that the character is not an entity.
                bool validEntity = false;


                // Identify whether the character represents a known entity.
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


                // If the character is an entity, store it in the room.
                if(validEntity)
                {
                    // Find the next available storage position.
                    int count = entityCounts[f][r];

                    // Make sure the room has not exceeded its storage capacity.
                    if(count >= MAX_ENTITIES_PER_ROOM)
                        return false;

                    // Store the entity and increase the room's entity count.
                    roomEntities[f][r][count] = symbol;

                    entityCounts[f][r]++;
                }
            }
        }
    }

    // All floors have been processed successfully.
    return true;
}


// Reads the initial game settings from the initial-state file.
bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft)
{
    // Open the initial-state file.
    ifstream file(filename);

    if(!file)
        return false;


    // Initialize the player's starting position.
    playerFloor = 0;
    playerRoom = 0;


    // Set the initial game values.
    energy = INITIAL_ENERGY;
    time = INITIAL_TIME;
    score = INITIAL_SCORE;

    inventoryCount = 0;
    emergencyTripsLeft = 0;


    // Empty all inventory positions.
    for(int i = 0; i < MAX_INVENTORY_CAPACITY; i++)
    {
        inventory[i] = ENT_NONE;
    }


    // Initially, all objectives are incomplete.
    for(int i = 0; i < NUM_OBJECTIVES; i++)
    {
        objectives[i] = false;
    }


    // Temporary storage for one line from the file.
    char line[MAX_LINE_LEN];


    // Read the file one line at a time.
    while(file.getline(line, MAX_LINE_LEN))
    {
        // Find the colon separating the setting name and its value.
        char* colon = strchr(line, ':');

        // Ignore lines that do not contain a colon.
        if(colon == NULL)
            continue;


        // Separate the setting name from its value.
        *colon = '\0';

        char* value = colon + 1;


        // Remove unnecessary whitespace from the setting name and value.
        trimWhitespace(line);
        trimWhitespace(value);


        // Read the initial energy value.
        if(stringEquals(line, "INITIAL_ENERGY"))
        {
            energy = 0;

            for(int i = 0; value[i] >= '0' && value[i] <= '9'; i++)
            {
                energy = energy * 10 + (value[i] - '0');
            }
        }


        // Read the initial time value.
        else if(stringEquals(line, "INITIAL_TIME"))
        {
            time = 0;

            for(int i = 0; value[i] >= '0' && value[i] <= '9'; i++)
            {
                time = time * 10 + (value[i] - '0');
            }
        }


        // Read the initial score value.
        else if(stringEquals(line, "INITIAL_SCORE"))
        {
            score = 0;

            for(int i = 0; value[i] >= '0' && value[i] <= '9'; i++)
            {
                score = score * 10 + (value[i] - '0');
            }
        }


        // Read the initial inventory.
        else if(stringEquals(line, "INITIAL_INVENTORY"))
        {
            if(stringEquals(value, "NONE"))
                inventoryCount = 0;
        }


        // Read the passenger rescue status.
        else if(stringEquals(line, "PASSENGER_RESCUED"))
        {
            objectives[OBJ_PASSENGER_RESCUED] = (stringEquals(value, "YES"));
        }


        // Read the cat rescue status.
        else if(stringEquals(line, "CAT_RESCUED"))
        {
            objectives[OBJ_CAT_SECURED] = (stringEquals(value, "YES"));
        }


        // Read the collected clues.
        else if(stringEquals(line, "CLUES_COLLECTED"))
        {
            if(stringEquals(value, "NONE"))
            {
                objectives[OBJ_CLUE_4] = false;
                objectives[OBJ_CLUE_2] = false;
                objectives[OBJ_CLUE_7] = false;
            }
        }


        // Read the cabinet unlock status.
        else if(stringEquals(line, "CABINET_UNLOCKED"))
        {
            objectives[OBJ_CABINET_UNLOCKED] = (stringEquals(value, "YES"));
        }


        // Read the train key collection status.
        else if(stringEquals(line, "TRAIN_KEY_COLLECTED"))
        {
            objectives[OBJ_TRAIN_KEY] = (stringEquals(value, "YES"));
        }


        // Read the fuse installation status.
        else if(stringEquals(line, "FUSE_INSTALLED"))
        {
            objectives[OBJ_FUSE_INSTALLED] = (stringEquals(value, "YES"));
        }


        // Read the lift pass collection status.
        else if(stringEquals(line, "LIFT_PASS_COLLECTED"))
        {
            objectives[OBJ_LIFT_PASS] = (stringEquals(value, "YES"));
        }


        // Read the train preparation status.
        else if(stringEquals(line, "TRAIN_PREPARED"))
        {
            objectives[OBJ_TRAIN_PREPARED] = (stringEquals(value, "YES"));
        }
    }

    // Close the initial-state file.
    file.close();

    return true;
}


// Checks whether the selected starting room has a usable horizontal connection.
bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
{
    // Check whether the selected floor and room are within valid limits.
    if(startFloor < 1 || startFloor > NUM_FLOORS)
        return false;

    if(startRoom < 1 || startRoom > ROOMS_PER_FLOOR)
        return false;


    // Convert the floor number from 1-based to 0-based indexing.
    int f = startFloor - 1;

    bool hasConnection = false;


    // Check the connection to the room on the left.
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


    // Check the connection to the room on the right.
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


    // A starting room is valid if it has at least one usable connection.
    return hasConnection;
}
