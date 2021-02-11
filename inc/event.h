#ifndef EVENT_H_
#define EVENT_H_

#include "bounded_buffer.hpp"
#include <stdint.h>
#include "CANIDs.h"

#include "leakCheck.h"
#include "canParseDefault.h"

void eventParse(bounded_buffer<struct can_frame>&);


#endif // EVENT_H_