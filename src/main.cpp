#include <iostream>
#include <thread>
#include <stdint.h>
#include "bounded_buffer.hpp"
#include "event.h"
#include "eventTimer.h"
#include "testThread.h"
#include "CANIDs.h"

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

int initLogger() {
    logging::add_file_log(
        keywords::file_name = "sample_%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = sinks::file::rotation_at_time_interval(boost::posix_time::hours(1)),
        keywords::format =
        (
            expr::stream
                << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                << " {" << expr::attr< boost::log::attributes::current_thread_id::value_type >("ThreadID") << "}"
                << ": <" << logging::trivial::severity
                << "> " << expr::smessage
        )
    );

    // Global filter settings
    //logging::core::get()->set_filter(
    //    logging::trivial::severity >= logging::trivial::info
    //);

    logging::add_console_log(
        std::cout, 
        keywords::filter = logging::trivial::severity >= logging::trivial::info,
        boost::log::keywords::format = (
            expr::stream
                << ">> " << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S")
                << ": <" << logging::trivial::severity
                << "> " << expr::smessage
        ));

    return 0;
}

int main () {
    bounded_buffer<struct can_frame> eventQueue(50);

    initLogger();
    logging::add_common_attributes();

    std::thread eventThread(eventParse, std::ref(eventQueue));

    std::thread testThreadThread(testing, std::ref(eventQueue));

    std::thread eventTimerThread(eventTimer, std::ref(eventQueue));

    eventThread.join();
    testThreadThread.join();
    eventTimerExit();
    eventTimerThread.join();

    return 0;
}

