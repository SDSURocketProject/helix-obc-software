#ifndef LEAK_CHECK_H_
#define LEAK_CHECK_H_

#include "CANIDs.h"
#include "Helix-OBC-Firmware.h"

/**
 * @defgroup stateLeakCheckGroup Leak Check
 * @brief Unique CAN message parsing for the leak check state.
 * 
 * @{
 */

uint32_t leakCheckInit(enum STATES (*canParseFunctions[CANIDS_EXTENDED_MAX]) (can_frame *));

/**
 * @} stateLeakCheckGroup
 */

#endif // LEAK_CHECK_H_