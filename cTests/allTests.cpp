#include <gtest/gtest.h>
extern "C" {
    #include "../src/counters.h"
    #include "../src/config.h"
}
// counters
ServiceErrorCounters* initEmptyCounters() {
    ServiceErrorCounters* counters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    counters = InitServiceErrorCounters(counters);
    return counters;
}

TEST(Counters, InitServiceErrorCounters) {
    ServiceErrorCounters* counters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    counters = InitServiceErrorCounters(counters);
    for(int i = 0; i < TraceOk; i++) {
        EXPECT_EQ(counters->statusCounter[i], 0);
    }
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        EXPECT_EQ(strlen(counters->myBadTraceExamples[i]), 0);
        EXPECT_EQ(strlen(counters->notmyBadTraceExamples[i]), 0);
    }
    EXPECT_EQ(counters->myExamplesCount, 0);
    EXPECT_EQ(counters->notmyExamplesCount, 0);
    // FreeServiceErrorCounters(counters);
}

TEST(Counters, FreeServiceErrorCounters) {
    ServiceErrorCounters* counters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    counters = initEmptyCounters();
    FreeServiceErrorCounters(counters);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}