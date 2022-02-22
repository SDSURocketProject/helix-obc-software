//
//  main.cpp
//  SocketCAN_readSocket
//
//  Created by Jean Michel Vives  on 2/21/22.
//
//
// This program reads data from the CAN bus via
// via the Linux socketCAN interface
//

#include <iostream>

// create a socket
int socket;

if ((socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
    perror("Socket");
}
