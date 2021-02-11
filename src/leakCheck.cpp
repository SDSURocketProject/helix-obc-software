#include "leakCheck.h"
#include "eventTimer.h"
#include <iostream>
#include <boost/log/trivial.hpp>

#include <thread>
#include <chrono>

static uint16_t heliumPressureStartLeakCheck;
static uint16_t heliumLastPressure;

static enum STATES checkHeliumPressure(can_frame *data) {
    int32_t pressureDifference = (int32_t)heliumPressureStartLeakCheck - (int32_t)heliumLastPressure;

    // If helium pressure dropped more than 10 psi
    if (pressureDifference > 10) {
        BOOST_LOG_TRIVIAL(warning) << "Failed leak check, helium dropped " << pressureDifference << "PSI.";
        return STATE_LEAK_CHECK;
    }
    else {
        BOOST_LOG_TRIVIAL(info) << "Passed helium leak check.";
    }

    return STATE_LEAK_CHECK;
}

/**
 * @addtogroup stateLeakCheckGroup
 * @{
 * 
 * @cond COMPILING_LATEX
 * @verbatim
 * 
 * \subsection{Custom Leak Check Actions}
 * 
 * @endverbatim
 * @endcond
 */

/**
 * @brief Parses Helium Pressure PT Data when received over the CAN Bus and in the leak check state.
 * @param[in] *data Contains the Helium Pressure PT Data CAN bus frame
 * @return Returns the state to move into
 * 
 * @cond COMPILING_LATEX
 * @verbatim CANID_DEF (Helium Pressure PT Data) @endverbatim
 * 
 * @endcond
 * 
 * @retval STATE_LEAK_CHECK Continue in the leak check state.
 * @retval STATE_IDLE Finished leak check so return to the idle state.
 * @retval STATE_GROUND_SAFE Return to the ground safe state because the helium tank pressure is overpressurized.
 * 
 * @cond COMPILING_LATEX
 * 
 * When Helium Pressure PT Data is received the CANID will be printed to stdout and the data will
 * be printed as a string to stdout. The current time and data with milliseconds is then printed
 * to stdout. The received can_frame is added to the eventTimer so that the received frame will
 * be receved again in 1 second. The system then continues on in the leak check state.
 * 
 * @verbatim END_CANID_DEF @endverbatim
 * 
 * @endcond
 * \n
 * The following will be done when Helium Pressure PTData is received while in the leak check state:
 * \n
 */
static enum STATES parseHeliumPressurePTData(can_frame *data) {
    //struct heliumPressurePtData *pressurePTData = data;
    //! Print out CANID and Data from the can_frame
    //std::cout << "CANID: " << data->can_id << "\n";
    //std::cout << "Data: " << data->data << "\n";

    struct heliumPressurePtData *canData = reinterpret_cast<struct heliumPressurePtData *>(data->data);

    BOOST_LOG_TRIVIAL(trace) << "CANID: " << data->can_id;
    BOOST_LOG_TRIVIAL(trace) << "Data: " << canData->heliumPressure;

    if (canData->heliumPressure > 300) {
        return STATE_GROUND_SAFE;
    }

    if (heliumPressureStartLeakCheck == 0) {
        BOOST_LOG_TRIVIAL(info) << "Started check on helium pressure, initial pressure: " << canData->heliumPressure;

        heliumPressureStartLeakCheck = canData->heliumPressure;
        struct can_frame checkHeliumPressureEvent;
        checkHeliumPressureEvent.can_id = CANIDS_LEAK_CHECK_HELIUM_PRESSURE_CHECK;
        // Check pressure again in 5 seconds
        eventTimerPushEvent(&checkHeliumPressureEvent, 5000);
    }

    heliumLastPressure = canData->heliumPressure;
    
    //! Continue in the leak check state.
    return STATE_LEAK_CHECK;
}

/**
 * @brief Called when entering the leak check state
 * @return Returns 0.
 * 
 * Prints a warning to stdout that the system is entering the leak check state.
 */
enum STATES leakCheckEnter(can_frame *data) {
    BOOST_LOG_TRIVIAL(info) << "Starting Leak Check.";
    // Start helium leak check on next helium PT measurement
    heliumPressureStartLeakCheck = 0;
    return STATE_LEAK_CHECK;
}

/**
 * @brief Called when exiting the leak check state
 * @return Returns 0.
 * 
 * Prints a warning to stdout that the system is exiting the leak check state.
 */
enum STATES leakCheckExit(can_frame *data) {
    BOOST_LOG_TRIVIAL(info) << "Exiting Leak Check.";
    // Remove all events that were intended for leak check
    eventTimerErase([](uint32_t cmpID)->bool {return (cmpID >= CANIDS_LEAK_CHECK_HELIUM_PRESSURE_CHECK);});
    return STATE_LEAK_CHECK;
}

/**
 * @brief Initialize leak check can parsing functions prior to starting event system
 * @param[out] *canParseFunctions List of pointers to functions that will be called when the associated CAN message is received.
 * 
 * The CAN Bus messages that need to be parsed uniquely to the leak check state are assigned to the canParse Functions array.
 */
uint32_t leakCheckInit(enum STATES (*canParseFunctions[CANIDS_EXTENDED_MAX]) (can_frame *)) {
    canParseFunctions[CANIDS_EXTENDED_STATE_ENTER] = leakCheckEnter;
    canParseFunctions[CANIDS_EXTENDED_STATE_EXIT] = leakCheckExit;
    
    canParseFunctions[CANIDS_HELIUM_PRESSURE_PT_DATA] = parseHeliumPressurePTData;
    canParseFunctions[CANIDS_LOX_PRESSURE_PT_DATA] = parseHeliumPressurePTData;
    canParseFunctions[CANIDS_LEAK_CHECK_HELIUM_PRESSURE_CHECK] = checkHeliumPressure;
    
    return 0;
}

/**
 * @} stateLeakCheckGroup
 */
