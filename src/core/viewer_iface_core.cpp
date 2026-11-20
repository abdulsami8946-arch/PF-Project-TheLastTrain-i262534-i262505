#include <iostream>
#include "viewer_iface.h"
#include "util.h"
using namespace std;

// Terminal UI and dashboard rendering using primitive parameters
void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft)
                     {
                        cout<<"======================"<<endl;
                        cout<<"=== THE LAST TRAIN ==="<<endl;
                        cout<<"======================"<<endl;
                        cout<<"Player Floor: "<<playerFloor<<endl;
                        cout<<"Player Room: "<<playerRoom<<endl;
                        cout<<"Energy: "<<energy<<endl;
                        cout<<"Time: "<<time<<endl;
                        cout<<"Score: "<<score<<endl;
                        cout<<"Inventory: ";
                        for (int i = 0; i < inventoryCount; i++) {
                            cout<<inventory[i]<<" ";
                        }
                        if (inventoryCount == 0) {
                            cout<<"(empty)";
                        }
                        cout<<endl;
                        cout<<"Emergency Trips Left: "<<emergencyTripsLeft<<endl;
                        cout<<"======================"<<endl;
                     }
void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount)
                    {
                        if (floor < 1 || floor > NUM_FLOORS || room < 1 || room > ROOMS_PER_FLOOR) 
                            {
                                cout<<"Invalid room location."<<endl;
                                return;
                            }
                        if (entityCount < 0 || entityCount > MAX_ENTITIES_PER_ROOM) 
                            {
                                cout<<"Invalid entity count."<<endl;
                                return;
                            }
                        cout<<"Room Details for Floor "<<floor<<", Room "<<room<<":"<<endl;
                        cout<<"Entities in the room: ";
                        for (int i = 0; i < entityCount; i++) 
                            {
                                cout<<roomEntities[floor-1][room-1][i]<<" ";
                            }
                        if (entityCount == 0) 
                            {
                                cout<<"(empty)";
                            }
                        cout<<endl;
                    }   
void renderAsciiBuilding(int playerFloor, int playerRoom)
                    {
                        cout<<"ASCII Building Representation:"<<endl;
                        for (int floor = NUM_FLOORS; floor >= 1; floor--) 
                            {
                                cout<<"Floor "<<floor<<": ";
                                for (int room = 1; room <= ROOMS_PER_FLOOR; room++) 
                                    {
                                        if (floor == playerFloor && room == playerRoom) 
                                            {
                                                cout<<"[P] "; // Player's current location
                                            } 
                                        else 
                                            {
                                                cout<<"[ ] "; // Empty room
                                            }
                                    }
                                cout<<endl;
                            }
                    }
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES])
                    {

                        cout<<"Objectives Checklist:"<<endl;
                        for (int i = 0; i < NUM_OBJECTIVES; i++) 
                            {
                                cout<<"Objective "<<(i+1)<<": ";
                                if (objectives[i]) 
                                    {
                                        cout<<"Completed"<<endl;
                                    } 
                                else 
                                    {
                                        cout<<"Incomplete"<<endl;
                                    }
                            }
                    }