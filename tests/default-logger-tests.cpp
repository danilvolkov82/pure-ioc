#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <sstream>

#include <default-logger.h>

class DefaultLoggerTest : public ::testing::Test {
protected:
    std::stringstream cout_buffer;
    std::stringstream cerr_buffer;
    std::streambuf* old_cout_buffer;
    std::streambuf* old_cerr_buffer;

    void SetUp() override {
        old_cout_buffer = std::cout.rdbuf(cout_buffer.rdbuf());
        old_cerr_buffer = std::cerr.rdbuf(cerr_buffer.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(old_cout_buffer);
        std::cerr.rdbuf(old_cerr_buffer);
    }
};

TEST_F(DefaultLoggerTest, Verbose) {
    auto logger = PureIOC::DefaultLogger();
    logger.verbose("test message");
    EXPECT_EQ(cout_buffer.str(), "[VERBOSE] test message\n");
    EXPECT_TRUE(cerr_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Info) {
    auto logger = PureIOC::DefaultLogger();
    logger.info("test message");
    EXPECT_EQ(cout_buffer.str(), "[INFO] test message\n");
    EXPECT_TRUE(cerr_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Debug) {
    auto logger = PureIOC::DefaultLogger();
    logger.debug("test message");
    EXPECT_EQ(cout_buffer.str(), "[DEBUG] test message\n");
    EXPECT_TRUE(cerr_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Warn) {
    auto logger = PureIOC::DefaultLogger();
    logger.warn("test message");
    EXPECT_EQ(cerr_buffer.str(), "[WARN] test message\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Error) {
    auto logger = PureIOC::DefaultLogger();
    logger.error("test message");
    EXPECT_EQ(cerr_buffer.str(), "[ERROR] test message\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Fatal) {
    auto logger = PureIOC::DefaultLogger();
    logger.fatal("test message");
    EXPECT_EQ(cerr_buffer.str(), "[FATAL] test message\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}
