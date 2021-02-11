#include "testThread.h"
#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>
#include "Helix-OBC-Firmware.h"
#include "eventTimer.h"
#include "CANIDs.h"

void testing(bounded_buffer<struct can_frame>& eventQueue) {
    struct can_frame data;

    BOOST_LOG_TRIVIAL(trace) << "Start test thread";

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    data.can_id = CANIDS_HELIUM_PRESSURE_PT_DATA;
    struct heliumPressurePtData canData;
    canData.heliumPressure = 100;

    for (unsigned int i = 0; i < 50*10; i++) {
        canData.timeSinceSystemStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
#if 0
        if (canData.heliumPressure < 320) {
            canData.heliumPressure += 2;
        }
#elif 0
        if (canData.heliumPressure > 80) {
            canData.heliumPressure -= 1;
        }
#endif
        memcpy(data.data, &canData, sizeof(struct heliumPressurePtData));

        eventQueue.push_front(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    data.can_id = CANIDS_EXTENDED_QUIT;
    eventQueue.push_front(data);
    BOOST_LOG_TRIVIAL(warning) << "End of testing thread";
}

