#include <gtest/gtest.h>
extern "C" {
    #include "../src/structures/counters.h"
    #include "../src/structures/span.h"
    #include "../src/structures/config.h"
    #include "../src/structures/trace.h"
    #include "../src/structures/service.h"
    #include "../src/structures/servicemap.h"
}

TEST(Counters, InitServiceErrorCounters) {
    ServiceErrorCounters* counters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    InitServiceErrorCounters(counters);
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
    InitServiceErrorCounters(counters);
    FreeServiceErrorCounters(counters);
}

TEST(Spans, InitSpan) {
    Span* span = (Span*)malloc(sizeof(Span));
    char* spanId = (char*)"spanId";
    char* serviceName = (char*)"serviceName";
    Span* parentSpan = NULL;
    InitSpan(span, spanId, serviceName, parentSpan);

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
    InitSpan(span, spanId, serviceName, parentSpan);
    FreeSpan(span);
}

TEST(Traces, InitTrace) {
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    char* traceString = (char*)"traceString";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    InitTrace(trace, traceString, serviceName, traceId);

    traceString = (char*)""; serviceName = (char*)""; traceId = (char*)"";

    EXPECT_EQ(strlen(trace->traceString), strlen("traceString"));
    EXPECT_EQ(strlen(trace->serviceName), strlen("serviceName"));
    EXPECT_EQ(strlen(trace->traceId), strlen("traceId"));
    // FreeTrace(trace);
}

TEST(Traces, FreeTrace) {
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    char* traceString = (char*)"traceString";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    InitTrace(trace, traceString, serviceName, traceId);
    FreeTrace(trace);
}

TEST(Service, InitService) {
    Service* service = (Service*)malloc(sizeof(Service));
    char* serviceName = (char*)"serviceName";
    InitService(service, serviceName);

    serviceName = (char*)"";

    EXPECT_EQ(strlen(service->serviceName), strlen("serviceName"));
    // FreeService(service);
}

TEST(Service, FreeService) {
    Service* service = (Service*)malloc(sizeof(Service));
    char* serviceName = (char*)"serviceName";
    InitService(service, serviceName);
    FreeService(service);
}

TEST(StringToService, InitStringToService) {
    struct StringToService* stringToService = (struct StringToService*)malloc(sizeof(struct StringToService));
    char* string = (char*)"string";
    Service* service = (Service*)malloc(sizeof(Service));
    InitService(service, string);
    InitStringToService(stringToService, string, service);

    string = (char*)"";

    EXPECT_EQ(strlen(stringToService->string), strlen("string"));
    // FreeStringToService(stringToService);
}

TEST(StringToService, FreeStringToService) {
    struct StringToService* stringToService = (struct StringToService*)malloc(sizeof(struct StringToService));
    char* string = (char*)"string";
    Service* service = (Service*)malloc(sizeof(Service));
    InitService(service, string);
    InitStringToService(stringToService, string, service);
    FreeStringToService(stringToService);
}

TEST(StringToService, AddNewService) {
    struct hashmap* stringToServiceMap = GetStringToServiceMap();
    char* serviceName = (char*)"serviceName";
    Service* service = AddNewService(stringToServiceMap, serviceName);
    EXPECT_EQ(strlen(service->serviceName), strlen("serviceName"));
    FreeStringToServiceMap(stringToServiceMap);
}

TEST(StringToService, FindAddedService) {
    struct hashmap* stringToServiceMap = GetStringToServiceMap();
    char* serviceName = (char*)"serviceName";
    Service* service = AddNewService(stringToServiceMap, serviceName);
    
    Service* foundService = FindService(stringToServiceMap, serviceName);

    EXPECT_EQ(strlen(foundService->serviceName), strlen("serviceName"));
    FreeStringToServiceMap(stringToServiceMap);
    
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}