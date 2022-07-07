#include <Arduino.h>
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <RHDatagram.h>

#include <definitions.h>

void updateRoutingTable(uint8_t nodeId, uint8_t routes[N_NODES], int16_t rssi[N_NODES], RHMesh* manager);
void getRouteInfoString(char *p, size_t len, uint8_t routes[N_NODES], int16_t rssi[N_NODES]);
void printNodeInfo(uint8_t node, char *s);