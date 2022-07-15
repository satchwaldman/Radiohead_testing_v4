// #include <Arduino.h>
// #include <RHRouter.h>
// #include <RHMesh.h>
// #include <RH_RF95.h>
// #include <RHDatagram.h>

// #include <definitions.h> //Gives this file access to N_NODES
// #include <findError.h>
// #include <routeInfo.h>

// uint64_t nodeId;
// uint8_t routes[N_NODES]; // full routing table for mesh
// int16_t rssi[N_NODES]; // signal strength info

// // Singleton instance of the radio driver
// RH_RF95 rf95(12, 6);

// // Class to manage message delivery and receipt, using the driver declared above
// RHMesh *manager;

// void setup() {
//   randomSeed(analogRead(0)); // initialize the random number generator with a fairly random input, such as analogRead() on an unconnected pin.
//   pinMode(LED, OUTPUT);
//   SerialUSB.begin(9600);
//   while (!SerialUSB) ; // Wait for SerialUSB port to be available

//   nodeId = 1; // CHANGE THIS FROM NODE TO NODE

//   SerialUSB.print(F("initializing node "));

//   manager = new RHMesh(rf95, nodeId);

//   if (!manager->init()) {
//     SerialUSB.println(F("init failed"));
//   } else {
//     SerialUSB.println("done");
//   }
//   rf95.setTxPower(23, false);
//   rf95.setFrequency(915.0);
//   rf95.setCADTimeout(500);

//   // Possible configurations:
//   // Bw125Cr45Sf128 (the chip default)
//   // Bw500Cr45Sf128
//   // Bw31_25Cr48Sf512
//   // Bw125Cr48Sf4096

//   // long range configuration requires for on-air time
//   boolean longRange = true;
//   if (longRange) {
//     RH_RF95::ModemConfig modem_config = {
//       0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
//       0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
//       0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
//     };
//     rf95.setModemRegisters(&modem_config);
//     if (!rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096)) {
//       SerialUSB.println(F("set config failed"));
//     }
//   }

//   SerialUSB.println("RF95 ready");

//   for(uint8_t n=1;n<=N_NODES;n++) { // initialize the routes[] and rssi[] arrays to 0
//     routes[n-1] = 0;
//     rssi[n-1] = 0;
//   }

// }

// char* buf = new char[RH_MESH_MAX_MESSAGE_LEN]; // define the buffer message, error message is eventually written here

// void loop() {

//   // message buffer (ONLY USED IN loop())
//   // char buf[RH_MESH_MAX_MESSAGE_LEN];

//   //n=sprintf (buf, "HELLO WORLD");
//   printf ("\"%s\" is the buffer message\n", buf);

//   for(uint8_t n=1;n<=N_NODES;n++) {
//     if (n == nodeId) continue; // self

//     updateRoutingTable(nodeId, routes, rssi, manager); // seems to only update routes (rssi[] appears to always be 0)
//     getRouteInfoString(buf, RH_MESH_MAX_MESSAGE_LEN, routes, rssi); // creates the [{}, {}, ...] string and writes it to buf

//     SerialUSB.print(F("->"));
//     SerialUSB.print(n);
//     SerialUSB.print(F(" :"));
//     SerialUSB.print(buf);

//     // send an acknowledged message to the target node
//     uint8_t error = manager->sendtoWait((uint8_t *)buf, strlen(buf), n);
//     if (error != RH_ROUTER_ERROR_NONE) {
//       SerialUSB.println();
//       SerialUSB.print(F(" ! "));
//       SerialUSB.println(getErrorString(error));
//     } else {
//       SerialUSB.println(F(" OK"));
//       // we received an acknowledgement from the next hop for the node we tried to send to.
//       RHRouter::RoutingTableEntry *route = manager->getRouteTo(n);
//       if (route->next_hop != 0) {
//         rssi[route->next_hop-1] = rf95.lastRssi();
//       }
//     }
//     if (nodeId == 1) printNodeInfo(nodeId, buf); // debugging

//     // listen for incoming messages. Wait a random amount of time before we transmit
//     // again to the next node
//     unsigned long nextTransmit = millis() + random(3000, 5000);
//     while (nextTransmit > millis()) {
//       int waitTime = nextTransmit - millis();
//       uint8_t len = sizeof(buf);
//       uint8_t from;
//       if (manager->recvfromAckTimeout((uint8_t *)buf, &len, waitTime, &from)) {
//         buf[len] = '\0'; // null terminate string
//         SerialUSB.print(from);
//         SerialUSB.print(F("->"));
//         SerialUSB.print(F(" :"));
//         SerialUSB.println(buf);
//         if (nodeId == 1) printNodeInfo(from, buf); // debugging (DOESN'T PASS ANY VARIABLES FROM definitions.h)
//         // we received data from node 'from', but it may have actually come from an intermediate node
//         RHRouter::RoutingTableEntry *route = manager->getRouteTo(from);
//         if (route->next_hop != 0) {
//           rssi[route->next_hop-1] = rf95.lastRssi();
//         }
//       }
//     }

    // ADDITIONAL CODE

    // send confirmation on all boards with nodeId > 1
    // bool signal_sent = false
    // while(signal_sent) {
    //   SerialUSB.println("confirmation from board " + String(n + 1));
    //   delay(5000);
    // }

    // SerialUSB.println("confirmation from board " + String(n + 1));
    // delay(5000);
    // create a 15 second delay, only on the board with nodeId = 1
    
    // delay(15000);
    // SerialUSB.println("hello world from board 1");
  
// }
// }


// -----------------------------------------------------------------------------
//                         Radiohead example code - RF95 
// -----------------------------------------------------------------------------


// ----------------------------- CLIENT (NodeID: 1) ----------------------------

// // rf95_client.pde
// // -*- mode: C++ -*-
// // Example sketch showing how to create a simple messageing client
// // with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// // reliability, so you should only use RH_RF95 if you do not need the higher
// // level messaging abilities.
// // It is designed to work with the other example rf95_server
// // Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with
// // the RFM95W, Adafruit Feather M0 with RFM95

// #include <SPI.h>
// #include <RH_RF95.h>

// // Singleton instance of the radio driver
// RH_RF95 rf95;
// //RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
// //RH_RF95 rf95(8, 3); // Adafruit Feather M0 with RFM95 

// // Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
// //#define Serial SerialUSB

// void setup() 
// {
//   // Rocket Scream Mini Ultra Pro with the RFM95W only:
//   // Ensure serial flash is not interfering with radio communication on SPI bus
// //  pinMode(4, OUTPUT);
// //  digitalWrite(4, HIGH);

//   Serial.begin(9600);
//   while (!Serial) ; // Wait for serial port to be available
//   if (!rf95.init())
//     Serial.println("init failed");
//   // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

//   // You can change the modulation parameters with eg
//   // rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  
//   // The default transmitter power is 13dBm, using PA_BOOST.
//   // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
//   // you can set transmitter powers from 2 to 20 dBm:
// //  rf95.setTxPower(20, false);
//   // If you are using Modtronix inAir4 or inAir9, or any other module which uses the
//   // transmitter RFO pins and not the PA_BOOST pins
//   // then you can configure the power transmitter power for 0 to 15 dBm and with useRFO true. 
//   // Failure to do that will result in extremely low transmit powers.
// //  rf95.setTxPower(14, true);
// }

// void loop()
// {
//   Serial.println("Sending to rf95_server");
//   // Send a message to rf95_server
//   uint8_t data[] = "Hello World!";
//   rf95.send(data, sizeof(data));
  
//   rf95.waitPacketSent();
//   // Now wait for a reply
//   uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//   uint8_t len = sizeof(buf);

//   if (rf95.waitAvailableTimeout(3000))
//   { 
//     // Should be a reply message for us now   
//     if (rf95.recv(buf, &len))
//    {
//       Serial.print("got reply: ");
//       Serial.println((char*)buf);
// //      Serial.print("RSSI: ");
// //      Serial.println(rf95.lastRssi(), DEC);    
//     }
//     else
//     {
//       Serial.println("recv failed");
//     }
//   }
//   else
//   {
//     Serial.println("No reply, is rf95_server running?");
//   }
//   delay(400);
// }


// ----------------------------- Server (NodeID: 2) ----------------------------


// // rf95_server.pde
// // -*- mode: C++ -*-
// // Example sketch showing how to create a simple messageing server
// // with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// // reliability, so you should only use RH_RF95  if you do not need the higher
// // level messaging abilities.
// // It is designed to work with the other example rf95_client
// // Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with
// // the RFM95W, Adafruit Feather M0 with RFM95

// #include <SPI.h>
// #include <RH_RF95.h>

// // Singleton instance of the radio driver
// RH_RF95 rf95;
// //RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
// //RH_RF95 rf95(8, 3); // Adafruit Feather M0 with RFM95 

// // Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
// //#define Serial SerialUSB

// int led = 9;

// void setup() 
// {
//   // Rocket Scream Mini Ultra Pro with the RFM95W only:
//   // Ensure serial flash is not interfering with radio communication on SPI bus
// //  pinMode(4, OUTPUT);
// //  digitalWrite(4, HIGH);

//   pinMode(led, OUTPUT);     
//   Serial.begin(9600);
//   while (!Serial) ; // Wait for serial port to be available
//   if (!rf95.init())
//     Serial.println("init failed");  
//   // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

//   // You can change the modulation parameters with eg
//   // rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);

// // The default transmitter power is 13dBm, using PA_BOOST.
//   // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
//   // you can set transmitter powers from 2 to 20 dBm:
// //  driver.setTxPower(20, false);
//   // If you are using Modtronix inAir4 or inAir9, or any other module which uses the
//   // transmitter RFO pins and not the PA_BOOST pins
//   // then you can configure the power transmitter power for 0 to 15 dBm and with useRFO true. 
//   // Failure to do that will result in extremely low transmit powers.
// //  driver.setTxPower(14, true);
// }

// void loop()
// {
//   if (rf95.available())
//   {
//     // Should be a message for us now   
//     uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//     uint8_t len = sizeof(buf);
//     if (rf95.recv(buf, &len))
//     {
//       digitalWrite(led, HIGH);
// //      RH_RF95::printBuffer("request: ", buf, len);
//       Serial.print("got request: ");
//       Serial.println((char*)buf);
// //      Serial.print("RSSI: ");
// //      Serial.println(rf95.lastRssi(), DEC);
      
//       // Send a reply
//       uint8_t data[] = "And hello back to you";
//       rf95.send(data, sizeof(data));
//       rf95.waitPacketSent();
//       Serial.println("Sent a reply");
//        digitalWrite(led, LOW);
//     }
//     else
//     {
//       Serial.println("recv failed");
//     }
//   }
// }


// --------------------------------------------------------------------------
//                              POINT TO POINT
// --------------------------------------------------------------------------


// ---------------------------- Server -------------------------------------

// #include <SPI.h>

// //Radio Head Library: 
// #include <RH_RF95.h>

// // We need to provide the RFM95 module's chip select and interrupt pins to the 
// // rf95 instance below.On the SparkFun ProRF those pins are 12 and 6 respectively.
// RH_RF95 rf95(12, 6);

// int LED = 13; //Status LED on pin 13

// int packetCounter = 0; //Counts the number of packets sent
// long timeSinceLastPacket = 0; //Tracks the time stamp of last packet received
// // The broadcast frequency is set to 921.2, but the SADM21 ProRf operates
// // anywhere in the range of 902-928MHz in the Americas.
// // Europe operates in the frequencies 863-870, center frequency at 
// // 868MHz.This works but it is unknown how well the radio configures to this frequency:
// //float frequency = 864.1;
// float frequency = 921.2;

// void setup()
// {
//   pinMode(LED, OUTPUT);

//   SerialUSB.begin(9600);
//   // It may be difficult to read serial messages on startup. The following
//   // line will wait for serial to be ready before continuing. Comment out if not needed.
//   while(!SerialUSB);
//   SerialUSB.println("RFM Server!");

//   //Initialize the Radio. 
//   if (rf95.init() == false){
//     SerialUSB.println("Radio Init Failed - Freezing");
//     while (1);
//   }
//   else{
//   // An LED indicator to let us know radio initialization has completed.
//     SerialUSB.println("Receiver up!");
//     digitalWrite(LED, HIGH);
//     delay(500);
//     digitalWrite(LED, LOW);
//     delay(500);
//   }

//   rf95.setFrequency(frequency); 

//  // The default transmitter power is 13dBm, using PA_BOOST.
//  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
//  // you can set transmitter powers from 5 to 23 dBm:
//  // rf95.setTxPower(14, false);
// }

// void loop()
// {
//   if (rf95.available()){
//     // Should be a message for us now
//     uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//     uint8_t len = sizeof(buf);

//     if (rf95.recv(buf, &len)){
//       digitalWrite(LED, HIGH); //Turn on status LED
//       timeSinceLastPacket = millis(); //Timestamp this packet

//       SerialUSB.print("Got message: ");
//       SerialUSB.print((char*)buf);
//       //SerialUSB.print(" RSSI: ");
//       //SerialUSB.print(rf95.lastRssi(), DEC);
//       SerialUSB.println();

//       // Send a reply
//       uint8_t toSend[] = "Hello Back!"; 
//       rf95.send(toSend, sizeof(toSend));
//       rf95.waitPacketSent();
//       SerialUSB.println("Sent a reply");
//       digitalWrite(LED, LOW); //Turn off status LED

//     }
//     else
//       SerialUSB.println("Recieve failed");
//   }
//   //Turn off status LED if we haven't received a packet after 1s
//   if(millis() - timeSinceLastPacket > 1000){
//     digitalWrite(LED, LOW); //Turn off status LED
//     timeSinceLastPacket = millis(); //Don't write LED but every 1s
//   }
// }

// ---------------------------- Client -------------------------------------

/*
  Both the TX and RX ProRF boards will need a wire antenna. We recommend a 3" piece of wire.
  This example is a modified version of the example provided by the Radio Head
  Library which can be found here:
  www.github.com/PaulStoffregen/RadioHeadd
*/

// #include <SPI.h>

// //Radio Head Library:
// #include <RH_RF95.h> 

// // We need to provide the RFM95 module's chip select and interrupt pins to the
// // rf95 instance below.On the SparkFun ProRF those pins are 12 and 6 respectively.
// RH_RF95 rf95(12, 6);

// int LED = 13; //Status LED is on pin 13

// int packetCounter = 0; //Counts the number of packets sent
// long timeSinceLastPacket = 0; //Tracks the time stamp of last packet received

// // The broadcast frequency is set to 921.2, but the SADM21 ProRf operates
// // anywhere in the range of 902-928MHz in the Americas.
// // Europe operates in the frequencies 863-870, center frequency at 868MHz.
// // This works but it is unknown how well the radio configures to this frequency:
// //float frequency = 864.1; 
// float frequency = 921.2; //Broadcast frequency

// void setup()
// {
//   pinMode(LED, OUTPUT);

//   SerialUSB.begin(9600);
//   // It may be difficult to read serial messages on startup. The following line
//   // will wait for serial to be ready before continuing. Comment out if not needed.
//   while(!SerialUSB); 
//   SerialUSB.println("RFM Client!"); 

//   //Initialize the Radio.
//   if (rf95.init() == false){
//     SerialUSB.println("Radio Init Failed - Freezing");
//     while (1);
//   }
//   else{
//     //An LED inidicator to let us know radio initialization has completed. 
//     SerialUSB.println("Transmitter up!"); 
//     digitalWrite(LED, HIGH);
//     delay(500);
//     digitalWrite(LED, LOW);
//     delay(500);
//   }

//   // Set frequency
//   rf95.setFrequency(frequency);

//    // The default transmitter power is 13dBm, using PA_BOOST.
//    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
//    // you can set transmitter powers from 5 to 23 dBm:
//    // Transmitter power can range from 14-20dbm.
//    rf95.setTxPower(14, false);
// }


// void loop()
// {
//   SerialUSB.println("Sending message");

//   //Send a message to the other radio
//   uint8_t toSend[] = "Hi there!";
//   //sprintf(toSend, "Hi, my counter is: %d", packetCounter++);
//   rf95.send(toSend, sizeof(toSend));
//   rf95.waitPacketSent();
//   //delay(5000);
//   // Now wait for a reply
//   byte buf[RH_RF95_MAX_MESSAGE_LEN];
//   byte len = sizeof(buf);

//   if (rf95.waitAvailableTimeout(2000)) {
//     // Should be a reply message for us now
//     if (rf95.recv(buf, &len)) {
//       SerialUSB.print("Got reply: ");
//       SerialUSB.println((char*)buf);
//       //SerialUSB.print(" RSSI: ");
//       //SerialUSB.print(rf95.lastRssi(), DEC);
//     }
//     else {
//       SerialUSB.println("Receive failed");
//     }
//   }
//   else {
//     SerialUSB.println("No reply, is the receiver running?");
//   }
//   delay(5000);
// }


// --------------------------------------------------------------------------
//               Radiohead example code - RF22 MESH CLIENT/SERVER
// --------------------------------------------------------------------------


// ---------------------------- Client -------------------------------------


// rf22_mesh_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, routed reliable messaging client
// with the RHMesh class.
// It is designed to work with the other examples rf22_mesh_server*
// Hint: you can simulate other network topologies by setting the 
// RH_TEST_NETWORK define in RHRouter.h

// Mesh has much greater memory requirements, and you may need to limit the
// max message length to prevent wierd crashes
#define RH_MESH_MAX_MESSAGE_LEN 50
#include <Arduino.h> // DEBUGGING
#include <RHMesh.h>
#include <RH_RF95.h> // CHANGED
#include <SPI.h>

// In this small artifical network of 4 nodes,
#define CLIENT_ADDRESS 1
#define SERVER1_ADDRESS 2
#define SERVER2_ADDRESS 3
// #define SERVER3_ADDRESS 4

// Singleton instance of the radio driver
RH_RF95 driver(12, 6); // CHANGED

// Class to manage message delivery and receipt, using the driver declared above
RHMesh manager(driver, CLIENT_ADDRESS);

void setup() 
{
  SerialUSB.begin(9600);
  //SerialUSB.print("client check");
  if (!manager.init())
    SerialUSB.println("init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36

    // DEBUGGING
  // pinMode(LED_BUILTIN, OUTPUT);
  // for (int n = 0; n < 5; n++) {
  //   SerialUSB.print("check number" + String(n + 1));
  //   digitalWrite(LED_BUILTIN, HIGH);
  // }
}

uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];

void loop()
{
  SerialUSB.println("Sending to manager_mesh_server3"); // UPDATE to the highest number server in the sequence
    
  // Send a message to a rf22_mesh_server
  // A route to the destination will be automatically discovered.

  // // ---------------------- SERVER1 -------------------

  // if (manager.sendtoWait(data, sizeof(data), SERVER1_ADDRESS) == RH_ROUTER_ERROR_NONE) // UPDATE?
  // {
  //   // It has been reliably delivered to the next node.
  //   // Now wait for a reply from the ultimate server
  //   uint8_t len = sizeof(buf);
  //   uint8_t from;    
  //   if (manager.recvfromAckTimeout(buf, &len, 3000, &from))
  //   {
  //     SerialUSB.print("got reply from : 0x");
  //     SerialUSB.print(from, HEX);
  //     SerialUSB.print(": ");
  //     SerialUSB.println((char*)buf);
  //   }
  //   else
  //   {
  //     SerialUSB.println("No reply, is rf22_mesh_server1, rf22_mesh_server2 and rf22_mesh_server3 running?");
  //   }
  // }
  // else
  //    SerialUSB.println("sendtoWait failed. Are the intermediate mesh servers running?");

  // // ---------------------- SERVER2 -------------------

  // if (manager.sendtoWait(data, sizeof(data), SERVER2_ADDRESS) == RH_ROUTER_ERROR_NONE) // UPDATE?
  // {
  //   // It has been reliably delivered to the next node.
  //   // Now wait for a reply from the ultimate server
  //   uint8_t len = sizeof(buf);
  //   uint8_t from;    
  //   if (manager.recvfromAckTimeout(buf, &len, 3000, &from))
  //   {
  //     SerialUSB.print("got reply from : 0x");
  //     SerialUSB.print(from, HEX);
  //     SerialUSB.print(": ");
  //     SerialUSB.println((char*)buf);
  //   }
  //   else
  //   {
  //     SerialUSB.println("No reply, is rf22_mesh_server1, rf22_mesh_server2 and rf22_mesh_server3 running?");
  //   }
  // }
  // else
  //    SerialUSB.println("sendtoWait failed. Are the intermediate mesh servers running?");

  // // ------------------------------------------------

  // implementation with for loop to access multiple servers

  //std::list<String> server_list {SERVER1_ADDRESS, SERVER2_ADDRESS};
  //server_list[] {SERVER1_ADDRESS, SERVER2_ADDRESS};
  int max_server;
  max_server = 3;

  for (int i = 2; i < max_server + 1; i++) {
    if (manager.sendtoWait(data, sizeof(data), i) == RH_ROUTER_ERROR_NONE) {// UPDATE?
    // It has been reliably delivered to the next node.
    // Now wait for a reply from the ultimate server
    uint8_t len = sizeof(buf);
    uint8_t from;    
    if (manager.recvfromAckTimeout(buf, &len, 3000, &from)) {
      SerialUSB.print("got reply from : 0x");
      SerialUSB.print(from, HEX);
      SerialUSB.print(": ");
      SerialUSB.println((char*)buf);
    }
    else {
      SerialUSB.println("No reply, is rf22_mesh_server1, rf22_mesh_server2 and rf22_mesh_server3 running?");
    }
  }
  else
    SerialUSB.println("sendtoWait failed. Are the intermediate mesh servers running?");
  }

  
}


//---------------------------- Server -------------------------------------


// // rf22_mesh_server1.pde
// // -*- mode: C++ -*-
// // Example sketch showing how to create a simple addressed, routed reliable messaging server
// // with the RHMesh class.
// // It is designed to work with the other examples rf22_mesh_*
// // Hint: you can simulate other network topologies by setting the 
// // RH_TEST_NETWORK define in RHRouter.h

// // Mesh has much greater memory requirements, and you may need to limit the
// // max message length to prevent wierd crashes
// #define RH_MESH_MAX_MESSAGE_LEN 50

// #include <RHMesh.h>
// #include <RH_RF95.h> // CHANGED
// #include <SPI.h>

// // In this small artifical network of 4 nodes,
// #define CLIENT_ADDRESS 1
// #define SERVER1_ADDRESS 2
// #define SERVER2_ADDRESS 3
// // #define SERVER3_ADDRESS 4

// // Singleton instance of the radio driver
// RH_RF95 driver(12, 6); // CHANGED

// // Class to manage message delivery and receipt, using the driver declared above
// RHMesh manager(driver, SERVER2_ADDRESS); // UPDATE

// void setup() 
// {
//   SerialUSB.begin(9600);
//   SerialUSB.print("server check");
//   if (!manager.init())
//     SerialUSB.println("RF22 init failed");
//   // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
// }

// uint8_t data[] = "And hello back to you from server2"; // UPDATE
// // Dont put this on the stack:
// uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
// void loop()
// {
//   uint8_t len = sizeof(buf);
//   uint8_t from;
//   if (manager.recvfromAck(buf, &len, &from))
//   {
//     SerialUSB.print("got request from : 0x");
//     SerialUSB.print(from, HEX);
//     SerialUSB.print(": ");
//     SerialUSB.println((char*)buf);

//     // Send a reply back to the originator client
//     if (manager.sendtoWait(data, sizeof(data), from) != RH_ROUTER_ERROR_NONE)
//       SerialUSB.println("sendtoWait failed");
//   }
//   delay(1000); // satch addition -- makes it easier to read
// }

