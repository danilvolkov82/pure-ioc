cmake_minimum_required(VERSION 3.10)
project(PureIoC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PURE_IOC_SOURCES
    src/container-manager.cpp
    src/default-logger.cpp
    src/default-services.cpp
    src/enable-logger-interface.cpp
    src/locator-mutable.cpp
    src/locator.cpp
)

set(PUBLIC_HEADERS
    src/container-manager.h
    src/enable-logger-interface.h
    src/locator-mutable.h
    src/locator.h
    src/logger-interface.h
    src/services-interface.h
)

set(PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
file(COPY ${PUBLIC_HEADERS} DESTINATION ${PUBLIC_INCLUDE_DIR})

set(SINGLE_HEADER_CONTENT "")
foreach(HEADER_FILE ${PUBLIC_HEADERS})
    get_filename_component(HEADER_NAME ${HEADER_FILE} NAME)
    set(SINGLE_HEADER_CONTENT "${SINGLE_HEADER_CONTENT}#include \"${HEADER_NAME}\"\n")
endforeach()

set(SINGLE_HEADER_GUARD "PURE_IOC_H")
set(SINGLE_HEADER_CONTENT "#ifndef ${SINGLE_HEADER_GUARD}\n#define ${SINGLE_HEADER_GUARD}\n\n${SINGLE_HEADER_CONTENT}\n#endif // ${SINGLE_HEADER_GUARD}\n")

set(SINGLE_HEADER_PATH "${PUBLIC_INCLUDE_DIR}/pure-ioc.h")
file(WRITE ${SINGLE_HEADER_PATH} "${SINGLE_HEADER_CONTENT}")

add_library(pure-ioc STATIC ${PURE_IOC_SOURCES})
set_target_properties(pure-ioc PROPERTIES OUTPUT_NAME pureioc++)

add_library(pure-ioc-shared SHARED ${PURE_IOC_SOURCES})
set_target_properties(pure-ioc-shared PROPERTIES OUTPUT_NAME pureioc++)

target_include_directories(pure-ioc
    PUBLIC
        ${PUBLIC_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(pure-ioc-shared
    PUBLIC
        ${PUBLIC_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
