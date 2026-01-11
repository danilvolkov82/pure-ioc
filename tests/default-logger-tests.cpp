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
    logger.verbose("TestTag", "test message");
    EXPECT_EQ(cout_buffer.str(), "[VERBOSE][TestTag] test message\n");
    EXPECT_TRUE(cerr_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Info) {
    auto logger = PureIOC::DefaultLogger();
    logger.info("TestTag", "test message");
    EXPECT_EQ(cout_buffer.str(), "[INFO][TestTag] test message\n");
    EXPECT_TRUE(cerr_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, InfoTemplateUsesTypeTag) {
    auto logger = PureIOC::DefaultLogger();
    logger.info<PureIOC::DefaultLogger>("test message");
    EXPECT_THAT(cout_buffer.str(), testing::StartsWith("[INFO]["));
    EXPECT_THAT(cout_buffer.str(), testing::EndsWith("] test message\n"));
    EXPECT_TRUE(cerr_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Debug) {
    auto logger = PureIOC::DefaultLogger();
    logger.debug("TestTag", "test message");
    EXPECT_EQ(cout_buffer.str(), "[DEBUG][TestTag] test message\n");
    EXPECT_TRUE(cerr_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Warn) {
    auto logger = PureIOC::DefaultLogger();
    logger.warn("TestTag", "test message");
    EXPECT_EQ(cerr_buffer.str(), "[WARN][TestTag] test message\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, WarnWithException) {
    auto logger = PureIOC::DefaultLogger();
    std::runtime_error error("boom");
    logger.warn("TestTag", "test message", error);
    EXPECT_EQ(cerr_buffer.str(), "[WARN][TestTag] test message Details: boom\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, WarnExceptionOnly) {
    auto logger = PureIOC::DefaultLogger();
    std::runtime_error error("boom");
    logger.warn("TestTag", error);
    EXPECT_EQ(cerr_buffer.str(), "[WARN][TestTag] boom\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Error) {
    auto logger = PureIOC::DefaultLogger();
    logger.error("TestTag", "test message");
    EXPECT_EQ(cerr_buffer.str(), "[ERROR][TestTag] test message\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, ErrorWithException) {
    auto logger = PureIOC::DefaultLogger();
    std::runtime_error error("boom");
    logger.error("TestTag", "test message", error);
    EXPECT_EQ(cerr_buffer.str(), "[ERROR][TestTag] test message Details: boom\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, ErrorExceptionOnly) {
    auto logger = PureIOC::DefaultLogger();
    std::runtime_error error("boom");
    logger.error("TestTag", error);
    EXPECT_EQ(cerr_buffer.str(), "[ERROR][TestTag] boom\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, Fatal) {
    auto logger = PureIOC::DefaultLogger();
    logger.fatal("TestTag", "test message");
    EXPECT_EQ(cerr_buffer.str(), "[FATAL][TestTag] test message\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, FatalWithException) {
    auto logger = PureIOC::DefaultLogger();
    std::runtime_error error("boom");
    logger.fatal("TestTag", "test message", error);
    EXPECT_EQ(cerr_buffer.str(), "[FATAL][TestTag] test message Details: boom\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}

TEST_F(DefaultLoggerTest, FatalExceptionOnly) {
    auto logger = PureIOC::DefaultLogger();
    std::runtime_error error("boom");
    logger.fatal("TestTag", error);
    EXPECT_EQ(cerr_buffer.str(), "[FATAL][TestTag] boom\n");
    EXPECT_TRUE(cout_buffer.str().empty());
}
