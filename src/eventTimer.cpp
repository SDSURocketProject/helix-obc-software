#include "eventTimer.h"
#include <queue>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <boost/circular_buffer.hpp>
#include <boost/log/trivial.hpp>

//!< Contains a can frame to be sent to the eventQueue after the wakeTime has passed.
struct timer_can_frame {
    struct can_frame canFrame;
    std::chrono::time_point<std::chrono::system_clock> wakeTime;
    // Custom comparison function for priority queue
    bool operator()(const struct timer_can_frame& lhs, const struct timer_can_frame& rhs) {
        if (lhs.wakeTime == rhs.wakeTime) {
            return lhs.canFrame.can_id < rhs.canFrame.can_id;
        }
        return lhs.wakeTime < rhs.wakeTime;
    }
};

//static std::priority_queue<struct timer_can_frame, std::vector<struct timer_can_frame>, timer_can_frame> timerQueue;
static boost::circular_buffer<struct timer_can_frame> timerQueue(50);
static std::mutex timerQueueMutex;
static std::condition_variable timerQueueCV;
std::atomic<bool> runEventTimer;
std::atomic<bool> skipPushEventTimer;

/**
 * @brief Event timer thread that pushes can_frames onto the the eventQueue at designated times.
 * @param[in] &eventQueue Queue containing all of the messages to be processed by the core event thread.
 * @return
 */
void eventTimer(bounded_buffer<struct can_frame>& eventQueue) {
    auto timerFrame = timerQueue.begin();

    BOOST_LOG_TRIVIAL(trace) << "Start eventTimer thread";

    runEventTimer = true;
    skipPushEventTimer = false;
    
    while (runEventTimer == true) {
        std::unique_lock<std::mutex> lk(timerQueueMutex);
        
        // Wait until the timerQueue is not empty
        timerQueueCV.wait(lk, []{return !timerQueue.empty();});

        // Check if the current thread needs to exit
        if (!runEventTimer) {
            break;
        }
        
        // Get the frame that needs to be sent the soonest
        timerFrame = timerQueue.begin();
        for (auto timerQueueIt = timerQueue.begin(); timerQueueIt < timerQueue.end(); timerQueueIt++) {
            if (timerQueueIt->wakeTime < timerFrame->wakeTime) {
                timerFrame = timerQueueIt;
            }
        }
        
        // Wait until it's either time to send the message or a new message has been put onto the priority queue
        timerQueueCV.wait_until(lk, timerFrame->wakeTime);
        
        if (skipPushEventTimer) {
            skipPushEventTimer = false;
            continue;
        }

        // Time has ellapsed and the timerQueue has not been cleared, send message
        if (std::chrono::system_clock::now() > timerFrame->wakeTime && !timerQueue.empty()) {
            eventQueue.push_front(timerFrame->canFrame);
            timerQueue.erase(timerFrame);
        }
    }
}

uint32_t eventTimerPushEvent(can_frame *frame, int milliseconds) {
    struct timer_can_frame timerFrame;
    timerFrame.canFrame = *frame;
    timerFrame.wakeTime = std::chrono::system_clock::now() + std::chrono::milliseconds(milliseconds);

    std::lock_guard<std::mutex> lk(timerQueueMutex);
    timerQueue.push_back(timerFrame);
    timerQueueCV.notify_one();
    return 0;
}

uint32_t eventTimerClear() {
    std::lock_guard<std::mutex> lk(timerQueueMutex);
    timerQueue.clear();
    timerQueueCV.notify_one();
    return 0;
}

uint32_t eventTimerErase(bool (*cmpFunc)(uint32_t)) {
    std::lock_guard<std::mutex> lk(timerQueueMutex);

    for (auto it = timerQueue.begin(); it < timerQueue.end(); it++) {
        if (cmpFunc((uint32_t)it->canFrame.can_id)) {
            timerQueue.erase(it--);
        }
    }
    
    timerQueueCV.notify_one();
    return 0;
}

uint32_t eventTimerExit() {
    runEventTimer = false;
    struct can_frame timerFrame;
    timerFrame.can_id = 0;
    // Push false event to get eventTimer thread out of waiting for data on queue
    eventTimerPushEvent(&timerFrame, 0);
    return 0;
}
