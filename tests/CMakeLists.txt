# For the tests, we need to link against gtest and gmock.
# We also need to include the source directory of the library.
add_executable(pure-ioc-tests
    container-manager-tests.cpp
    locator-mutable-tests.cpp
)

target_link_libraries(pure-ioc-tests
    GTest::gtest_main
    GTest::gmock
    pure-ioc
)

include(GoogleTest)
gtest_discover_tests(pure-ioc-tests)
