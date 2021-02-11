#include "event.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>
#include "Helix-OBC-Firmware.h"
#include "leakCheck.h"
#include "groundSafeState.h"

uint32_t canParseNothing(can_frame *data) {
    BOOST_LOG_TRIVIAL(info) << "canParseNothing called with id " << data->can_id;
    return 0;
}

// A 2d array containing function pointers for each canID for each possible state
static enum STATES (*canParseFunctions[STATE_MAX_STATES][CANIDS_EXTENDED_MAX]) (can_frame *);
// An array of function pointers for default processing of canIDs
static uint32_t (*canParseFunctionsDefault[CANIDS_EXTENDED_MAX]) (can_frame *);

static uint32_t checkInitFunction(enum STATES state, std::string stateName, uint32_t result) {
    if (result > 0) {
        BOOST_LOG_TRIVIAL(error) << stateName << " init Error";
        return 1;
    }
    else if (canParseFunctions[state][CANIDS_EXTENDED_STATE_ENTER] == nullptr) {
        BOOST_LOG_TRIVIAL(error) << stateName << " state enter callback not initialized";
        return 1;
    }
    else if (canParseFunctions[state][CANIDS_EXTENDED_STATE_EXIT] == nullptr) {
        BOOST_LOG_TRIVIAL(error) << stateName << " state exit callback not initialized";
        return 1;
    }
    else {
        BOOST_LOG_TRIVIAL(info) << stateName << " CAN parser init completed without error.";
        return 0;
    }
}

void eventParse(bounded_buffer<struct can_frame>& eventQueue) {
    uint32_t result = 0;

    BOOST_LOG_TRIVIAL(trace) << "Start event thread";

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Set default parsing functions
    for (uint32_t canID = 0; canID < CANIDS_EXTENDED_MAX; canID++) {
        canParseFunctionsDefault[canID] = canParseNothing;
    }

    // Use default parsing for can messages
    for (uint32_t state = STATE_IDLE; state < STATE_MAX_STATES; state++) {
        for (uint32_t canID = 0; canID < CANIDS_EXTENDED_MAX; canID++) {
            // If callback points to null it is assumed the functionality in not implemented
            canParseFunctions[state][canID] = NULL;
        }
    }

    result = leakCheckInit(canParseFunctions[STATE_LEAK_CHECK]);
    result = checkInitFunction(STATE_LEAK_CHECK, "Leak check", result);
    if (result != 0) {
        // exit?
    }

    result = groundSafeStateInit(canParseFunctions[STATE_GROUND_SAFE]);
    result = checkInitFunction(STATE_GROUND_SAFE, "Ground Safe State", result);
    if (result != 0) {
        // exit?
    }
    
    struct can_frame event;
    enum STATES currentState = STATE_LEAK_CHECK;
    enum STATES nextState = STATE_LEAK_CHECK;

    while (event.can_id != CANIDS_EXTENDED_QUIT) {
        // Get next item off can event queue
        eventQueue.pop_back(&event);

        // Check flags from CAN Bus transaction
        if (event.can_id & CAN_EFF_FLAG) {
            // EFF/SFF is set in the MSB
        }
        else if (event.can_id & CAN_RTR_FLAG) {
            // remote transmission request
        }
        else if (event.can_id & CAN_ERR_FLAG) {
            // error message frame
            continue;
        }

        // Check if current state implements functionality for the received CAN event
        if (canParseFunctions[currentState][event.can_id] != NULL) {
            nextState = canParseFunctions[currentState][event.can_id](&event);
        }
        else {
            canParseFunctionsDefault[event.can_id](&event);
        }

        // If transitioning between states call the corresponding exit state callback and enter state callback
        if (nextState != currentState) {
            canParseFunctions[currentState][CANIDS_EXTENDED_STATE_EXIT](nullptr);
            canParseFunctions[nextState][CANIDS_EXTENDED_STATE_ENTER](nullptr);
            currentState = nextState;
        }
    }
}

