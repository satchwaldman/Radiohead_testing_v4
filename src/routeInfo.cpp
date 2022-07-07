#include <Arduino.h>
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <RHDatagram.h>

#include <definitions.h> //Gives this file access to N_NODES
#include <routeInfo.h>

// uint8_t nodeId;
// uint8_t routes[N_NODES]; 
// int16_t rssi[N_NODES]; 

// uint8_t nodeId;
// uint8_t routes[N_NODES]; // full routing table for mesh
// int16_t rssi[N_NODES]; // signal strength info

// // Singleton instance of the radio driver
// RH_RF95 rf95(12, 6);

// // Class to manage message delivery and receipt, using the driver declared above
// RHMesh *manager;

// // message buffer
// char buf[RH_MESH_MAX_MESSAGE_LEN];


void updateRoutingTable(uint8_t nodeId, uint8_t routes[N_NODES], int16_t rssi[N_NODES], RHMesh* manager) {
  for(uint8_t n=1;n<=N_NODES;n++) {
    RHRouter::RoutingTableEntry *route = manager->getRouteTo(n);
    if (n == nodeId) {
      routes[n-1] = 255; // self
    } else {
      routes[n-1] = route->next_hop;
      if (routes[n-1] == 0) {
        // if we have no route to the node, reset the received signal strength
        rssi[n-1] = 0;
      }
    }
  }
}

// Create a JSON string with the routing info to each node
void getRouteInfoString(char *p, size_t len, uint8_t routes[N_NODES], int16_t rssi[N_NODES]) {
  p[0] = '\0';
  strcat(p, "[");
  for(uint8_t n=1;n<=N_NODES;n++) {
    strcat(p, "{\"n\":");
    sprintf(p+strlen(p), "%d", routes[n-1]);
    strcat(p, ",");
    strcat(p, "\"r\":");
    sprintf(p+strlen(p), "%d", rssi[n-1]);
    strcat(p, "}");
    if (n<N_NODES) {
      strcat(p, ",");
    }
  }
  strcat(p, "]");
}

void printNodeInfo(uint8_t node, char *s) {
  SerialUSB.print(F("node: "));
  SerialUSB.print(F("{"));
  SerialUSB.print(F("\""));
  SerialUSB.print(node);
  SerialUSB.print(F("\""));
  SerialUSB.print(F(": "));
  SerialUSB.print(s);
  SerialUSB.println(F("}"));
}