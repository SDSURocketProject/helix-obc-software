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

// used http://www.microhowto.info/howto/get_the_index_number_of_a_linux_network_interface_in_c_using_siocgifindex.html


#include <can.h> // for can_frame
#include <linux/can.h>

//    Opens A Socket and Bind

// opens socket with protocolamily can, raw socket data, and bind can_raw
int socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

// checks if there is an error
if (socket < 0) {
    perror("Socket");
}

// 1) Create ifreq sturct for passing data in and out of struct
struct ifreq ifr;

// 2) strcpy copies c string pointed by source into the array pointed by destination
// strcpy(dest, source) source into destination
strcpy(ifr.ifr_name, "vcan0");

// 3) Invoke ioctl (in out control)
// index # of network interface obtained using ioctl command SIOCGIFINDEX
// SIOCGIFINDEX = Given the ifr_ifindex, return the name of the interface in ifr_name.
ioctl (socket, SIOCGIFINDEX, &ifr);



//    Reading A Frame

// to recieve a CAN frame, init can_frame struct and populate with data
struct can_frame frame;

// reads sizeof(frame) bytes from socket into buffer pointed by &frame
nbytes = read(socket, &frame, sizeof(frame));

frame
