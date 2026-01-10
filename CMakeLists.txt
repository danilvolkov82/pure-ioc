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

file(GLOB PUBLIC_HEADERS "src/*.h")

add_library(pure-ioc STATIC ${PURE_IOC_SOURCES})
set_target_properties(pure-ioc PROPERTIES OUTPUT_NAME pureioc++)

add_library(pure-ioc-shared SHARED ${PURE_IOC_SOURCES})
set_target_properties(pure-ioc-shared PROPERTIES OUTPUT_NAME pureioc++)

target_include_directories(pure-ioc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(pure-ioc-shared
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS pure-ioc pure-ioc-shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${PUBLIC_HEADERS} DESTINATION include/pure-ioc)

set(SINGLE_HEADER_CONTENT "#ifndef PURE_IOC_H\n#define PURE_IOC_H\n\n")
foreach(HEADER_FILE ${PUBLIC_HEADERS})
    get_filename_component(HEADER_NAME ${HEADER_FILE} NAME)
    string(APPEND SINGLE_HEADER_CONTENT "#include \"pure-ioc/${HEADER_NAME}\"\n")
endforeach()
string(APPEND SINGLE_HEADER_CONTENT "\n#endif // PURE_IOC_H\n")

file(WRITE "${CMAKE_BINARY_DIR}/pure-ioc.h" "${SINGLE_HEADER_CONTENT}")
install(FILES "${CMAKE_BINARY_DIR}/pure-ioc.h" DESTINATION include)


option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    include(cmake/GTest.cmake)
    add_subdirectory(tests)
endif()
