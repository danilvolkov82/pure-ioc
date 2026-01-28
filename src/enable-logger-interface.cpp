/**
 * @file enable-logger-interface.cpp
 * @brief This file contains the implementation of the IEnableLogger interface.
 */

#include "enable-logger-interface.h"
#include "locator.h"
#include "locator-mutable.h"
#include "internal/default-logger.h"

using namespace PureIOC;

std::shared_ptr<ILogger>
IEnableLogger::logger() {
    std::shared_ptr<ILogger> logger = getService<ILogger>();
    if(!logger) {
        registerLogger<DefaultLogger>();
        logger = getService<ILogger>();
    }

    return logger;
}