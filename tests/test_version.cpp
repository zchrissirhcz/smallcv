#include "version.h"
#include "gtest/gtest.h"

TEST(version, macros)
{
    EXPECT_TRUE(SMALLCV_VERSION_MAJOR >= 0);
    EXPECT_TRUE(SMALLCV_VERSION_MINOR >= 0);
    EXPECT_TRUE(SMALLCV_VERSION_PATCH >= 0);
}

