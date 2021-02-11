#ifndef GROUND_SAFE_SAFE_H_
#define GROUND_SAFE_SAFE_H_

#include "CANIDs.h"
#include "Helix-OBC-Firmware.h"

uint32_t groundSafeStateInit(enum STATES (*canParseFunctions[CANIDS_EXTENDED_MAX]) (can_frame *));

#endif // GROUND_SAFE_SAFE_H_