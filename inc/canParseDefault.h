#ifndef CANPARSEDEFAULT_H_
#define CANPARSEDEFAULT_H_

#include "CANIDs.h"

uint32_t leakCheckInit(uint32_t (*canParseFunctions[CANIDS_MAX_CANID]) (can_frame *));

#endif // CANPARSEDEFAULT_H_