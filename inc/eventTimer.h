#ifndef EVENTTIMER_H_
#define EVENTTIMER_H_

#include "bounded_buffer.hpp"
#include <linux/can.h>
#include <stdint.h>
#include <atomic>

void eventTimer(bounded_buffer<struct can_frame>&);
uint32_t eventTimerPushEvent(can_frame *, int);
uint32_t eventTimerClear();
uint32_t eventTimerErase(bool (*cmpFunc)(uint32_t));
uint32_t eventTimerExit();

#endif // EVENTTIMER_H_