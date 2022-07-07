#include <Arduino.h>
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <RHDatagram.h>

#include <definitions.h> //Gives this file access to N_NODES
#include <findError.h>
#include <routeInfo.h>

uint64_t nodeId;
uint8_t routes[N_NODES]; // full routing table for mesh
int16_t rssi[N_NODES]; // signal strength info

// Singleton instance of the radio driver
RH_RF95 rf95(12, 6);

// Class to manage message delivery and receipt, using the driver declared above
RHMesh *manager;

void setup() {
  randomSeed(analogRead(0));
  pinMode(LED, OUTPUT);
  SerialUSB.begin(9600);
  while (!SerialUSB) ; // Wait for SerialUSB port to be available

  nodeId = 2; // CHANGE THIS FROM NODE TO NODE

  SerialUSB.print(F("initializing node "));

  manager = new RHMesh(rf95, nodeId);

  if (!manager->init()) {
    SerialUSB.println(F("init failed"));
  } else {
    SerialUSB.println("done");
  }
  rf95.setTxPower(23, false);
  rf95.setFrequency(915.0);
  rf95.setCADTimeout(500);

  // Possible configurations:
  // Bw125Cr45Sf128 (the chip default)
  // Bw500Cr45Sf128
  // Bw31_25Cr48Sf512
  // Bw125Cr48Sf4096

  // long range configuration requires for on-air time
  boolean longRange = true;
  if (longRange) {
    RH_RF95::ModemConfig modem_config = {
      0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
      0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
      0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
    };
    rf95.setModemRegisters(&modem_config);
    if (!rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096)) {
      SerialUSB.println(F("set config failed"));
    }
  }

  SerialUSB.println("RF95 ready");

  for(uint8_t n=1;n<=N_NODES;n++) {
    routes[n-1] = 0;
    rssi[n-1] = 0;
  }

}

char* buf = new char[RH_MESH_MAX_MESSAGE_LEN];

void loop() {

  // message buffer (ONLY USED IN loop())
  // char buf[RH_MESH_MAX_MESSAGE_LEN];

  n=sprintf (buf, "HELLO WORLD");
  printf ("\"%s\" is the buffer message\n", buf);

  for(uint8_t n=1;n<=N_NODES;n++) {
    if (n == nodeId) continue; // self

    updateRoutingTable(nodeId, routes, rssi, manager);
    getRouteInfoString(buf, RH_MESH_MAX_MESSAGE_LEN, routes, rssi);

    SerialUSB.print(F("->"));
    SerialUSB.print(n);
    SerialUSB.print(F(" :"));
    SerialUSB.print(buf);

    // send an acknowledged message to the target node
    uint8_t error = manager->sendtoWait((uint8_t *)buf, strlen(buf), n);
    if (error != RH_ROUTER_ERROR_NONE) {
      SerialUSB.println();
      SerialUSB.print(F(" ! "));
      SerialUSB.println(getErrorString(error));
    } else {
      SerialUSB.println(F(" OK"));
      // we received an acknowledgement from the next hop for the node we tried to send to.
      RHRouter::RoutingTableEntry *route = manager->getRouteTo(n);
      if (route->next_hop != 0) {
        rssi[route->next_hop-1] = rf95.lastRssi();
      }
    }
    if (nodeId == 1) printNodeInfo(nodeId, buf); // debugging

    // listen for incoming messages. Wait a random amount of time before we transmit
    // again to the next node
    unsigned long nextTransmit = millis() + random(3000, 5000);
    while (nextTransmit > millis()) {
      int waitTime = nextTransmit - millis();
      uint8_t len = sizeof(buf);
      uint8_t from;
      if (manager->recvfromAckTimeout((uint8_t *)buf, &len, waitTime, &from)) {
        buf[len] = '\0'; // null terminate string
        SerialUSB.print(from);
        SerialUSB.print(F("->"));
        SerialUSB.print(F(" :"));
        SerialUSB.println(buf);
        if (nodeId == 1) printNodeInfo(from, buf); // debugging (DOESN'T PASS ANY VARIABLES FROM definitions.h)
        // we received data from node 'from', but it may have actually come from an intermediate node
        RHRouter::RoutingTableEntry *route = manager->getRouteTo(from);
        if (route->next_hop != 0) {
          rssi[route->next_hop-1] = rf95.lastRssi();
        }
      }
    }

    // ADDITIONAL CODE

    // send confirmation on all boards with nodeId > 1
    // bool signal_sent = false
    // while(signal_sent) {
    //   SerialUSB.println("confirmation from board " + String(n + 1));
    //   delay(5000);
    // }

    SerialUSB.println("confirmation from board " + String(n + 1));
    delay(5000);
    // create a 15 second delay, only on the board with nodeId = 1
    
    // delay(15000);
    // SerialUSB.println("hello world from board 1");
  
}
}