#ifndef TESTTHREAD_H_
#define TESTTHREAD_H_

#include "bounded_buffer.hpp"
#include <stdint.h>
#include "CANIDs.h"

void testing(bounded_buffer<struct can_frame>&);

#endif // TESTTHREAD_H_