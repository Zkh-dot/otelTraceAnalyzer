#include <gtest/gtest.h>
extern "C" {
    #include "../src/counters.h"
    #include "../src/span.h"
    #include "../src/config.h"
    #include "../src/trace.h"
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

TEST(Spans, InitSpan) {
    Span* span = (Span*)malloc(sizeof(Span));
    char* spanId = (char*)"spanId";
    char* serviceName = (char*)"serviceName";
    Span* parentSpan = NULL;
    span = InitSpan(span, spanId, serviceName, parentSpan);

    spanId = (char*)""; serviceName = (char*)"";

    EXPECT_EQ(strlen(span->spanId), strlen("spanId"));
    EXPECT_EQ(strlen(span->serviceName), strlen("serviceName"));
    EXPECT_EQ(span->spanStatus, UndefSpanStatus);
    // FreeSpan(span);
}

TEST(Spans, FreeSpan) {
    Span* span = (Span*)malloc(sizeof(Span));
    char* spanId = (char*)"spanId";
    char* serviceName = (char*)"serviceName";
    Span* parentSpan = NULL;
    span = InitSpan(span, spanId, serviceName, parentSpan);
    FreeSpan(span);
}

TEST(Traces, InitTrace) {
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    char* traceString = (char*)"traceString";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    trace = InitTrace(trace, traceString, serviceName, traceId);

    traceString = (char*)""; serviceName = (char*)""; traceId = (char*)"";

    EXPECT_EQ(strlen(trace->traceString), strlen("traceString"));
    EXPECT_EQ(strlen(trace->serviceName), strlen("serviceName"));
    EXPECT_EQ(strlen(trace->traceId), strlen("traceId"));
    // FreeTrace(trace);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}