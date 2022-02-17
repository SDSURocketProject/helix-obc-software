// Developed by Eli Reece
//https://github.com/Eli-Reece

#include <SPI.h>
#define CAN_2515

/*
set up the SPI and CAN PIN
*/
#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else
// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#endif
#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif
#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);                     //Serial Monitor Baud Rate
    pinMode(12,INPUT_PULLUP);                              //BUTTON
    while(!Serial){};

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}

void loop() {
    if(digitalRead(12) == LOW) {                          //if button is pressed
      // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
      CAN.sendMsgBuf(0xC1, 0, 1, 0x01);                   //send button pressed data to slave
      SERIAL_PORT_MONITOR.println("CAN BUS sendMsgBuf ok! -> button pressed");
      delay(100);                                         // send data per 100ms
    } else {                                              //if button is not pressed
      CAN.sendMsgBuf(0xC1, 0, 1, 0x00);                   //send button is not pressed data to slave
      SERIAL_PORT_MONITOR.println("CAN BUS sendMsgBuf ok! -> button not pressed");
      delay(100);                                         // send data per 100ms
    }
}
