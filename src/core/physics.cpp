#include "physics.h"
#include "util.h"
bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,char itemSymbol){
    for(int i = 0; i < inventoryCount; i++)
    {
        if(inventory[i] == itemSymbol)
        {
            return true;
        }
    }
    return false;
}
bool addItem(char inventory[MAX_INVENTORY_CAPACITY],
             int& inventoryCount,
             char itemSymbol)
{
    if(inventoryCount >= MAX_INVENTORY_CAPACITY)
    {
        return false;
    }
    inventory[inventoryCount] = itemSymbol;
    inventoryCount++;
    return true;
}
bool removeItem(char inventory[MAX_INVENTORY_CAPACITY],
                int& inventoryCount,
                char itemSymbol)
{
    for(int i = 0; i < inventoryCount; i++)
    {
        if(inventory[i] == itemSymbol)
        {
            for(int j = i; j < inventoryCount - 1; j++)
            {
                inventory[j] = inventory[j + 1];
            }

            inventoryCount--;
            return true;
        }
    }
    return false;
}
void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime)
{
    energy += deltaEnergy;
    time += deltaTime;

    energy = clampInt(energy, 0, MAX_ENERGY);
    time = clampInt(time, 0, MAX_TIME);
}
void applyScoreDelta(int& score, int deltaScore)
{
    score += deltaScore;

    if(score < 0)
        score = 0;
}
bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType)
{
    outConnType = CONN_NONE;

    if(fromF < 1 || fromF > NUM_FLOORS ||
       toF < 1 || toF > NUM_FLOORS ||
       fromR < 1 || fromR > ROOMS_PER_FLOOR ||
       toR < 1 || toR > ROOMS_PER_FLOOR)
    {
        return false;
    }

    if(fromF != toF)
        return false;

    if(fromR - toR != 1 && toR - fromR != 1)
        return false;

    int connectionIndex;

    if(fromR < toR)
        connectionIndex = fromR - 1;
    else
        connectionIndex = toR - 1;

    outConnType = horizontalConn[fromF - 1][connectionIndex];

    if(outConnType == CONN_NONE)
        return false;

    return true;
}
bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational)
{
    if(targetFloor < 1 || targetFloor > NUM_FLOORS ||
       targetRoom < 1 || targetRoom > ROOMS_PER_FLOOR)
    {
        return false;
    }

    if(inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
        return false;

    if(connType == CONN_DOOR)
        return true;

    if(connType == CONN_WINDOW)
        return true;

    if(connType == CONN_LOCKED_DOOR)
        return hasItem(inventory, inventoryCount, ENT_MASTER_KEY);

    if(connType == CONN_SOLID_WALL)
        return false;

    if(connType == 'S')
    {
        if(targetRoom == 1)
            return true;

        return false;
    }

    if(connType == '1')
    {
        if((targetFloor == 1 || targetFloor == 2) &&
           (targetRoom == 2 || targetRoom == 3))
        {
            return true;
        }

        return false;
    }

    if(connType == '2')
    {
        if(targetRoom != 4 ||
           (targetFloor != 2 && targetFloor != 4 && targetFloor != 5))
        {
            return false;
        }

        if(lift2Operational)
        {
            return hasItem(inventory, inventoryCount, ENT_LIFT_PASS);
        }

        if(emergencyTripsLeft > 0)
        {
            return hasItem(inventory, inventoryCount, ENT_EMERGENCY_CARD);
        }

        return false;
    }

    return false;
}
// Movement validation and resource adjustments without structures
