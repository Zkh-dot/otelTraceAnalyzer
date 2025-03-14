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
    char* parentSpanId = (char*)"parentSpanId";
    Span* parentSpan = NULL;
    InitSpan(span, spanId, serviceName, parentSpanId, parentSpan);

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
    char* parentSpanId = (char*)"parentSpanId";
    Span* parentSpan = NULL;
    InitSpan(span, spanId, serviceName, parentSpanId, parentSpan);
    FreeSpan(span);
}

TEST(Spans, FreeSpanTree) {
    Span* span = (Span*)malloc(sizeof(Span));
    char* spanId = (char*)"spanId";
    char* serviceName = (char*)"serviceName";
    Span* parentSpan = (Span*)malloc(sizeof(Span));
    char* parentSpanId = (char*)"parentSpanId";
    char* parentServiceName = (char*)"parentServiceName";
    InitSpan(span, spanId, serviceName, parentSpanId, parentSpan);
    InitSpan(parentSpan, parentSpanId, parentServiceName, NULL, NULL);
    EXPECT_EQ(strlen(span->spanId), strlen("spanId"));
    EXPECT_EQ(strlen(span->serviceName), strlen("serviceName"));
    EXPECT_EQ(span->spanStatus, UndefSpanStatus);
    EXPECT_EQ(strlen(span->parentSpan->spanId), strlen("parentSpanId"));
    EXPECT_EQ(strlen(span->parentSpan->serviceName), strlen("parentServiceName"));
    EXPECT_EQ(span->parentSpan->spanStatus, UndefSpanStatus);
    FreeSpanTree(span);
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

TEST(Traces, ScanTrace) {
    const char* traceString = (char*)"{'traceId': 'traceId', 'serviceName': 'serviceName', 'spans':[{'spanId': 'spanId', 'serviceName': 'serviceName'}]}";
    char* serviceName = ScanTrace("serviceName", traceString);
    EXPECT_EQ(strlen(serviceName), strlen("serviceName"));
}

TEST(Traces, FindAllSpans) {
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    char* traceString = (char*)"[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    InitTrace(trace, traceString, serviceName, traceId);
    FindAllSpans(trace);
    EXPECT_EQ(strlen(trace->spans[0]->spanId), 16);
    FreeTrace(trace);
}

TEST(Spans, FindById) {
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    char* traceString = (char*)"[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    InitTrace(trace, traceString, serviceName, traceId);
    FindAllSpans(trace);
    Span* span = FindSpan(trace->spans, trace->spansCount, (char*)"0000000000000000");
    EXPECT_EQ(strlen(span->spanId), 16);
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
    struct StringToService* StringToService = (struct StringToService*)malloc(sizeof(struct StringToService));
    char* string = (char*)"string";
    Service* service = (Service*)malloc(sizeof(Service));
    InitService(service, string);
    InitStringToService(StringToService, string, service);

    string = (char*)"";

    EXPECT_EQ(strlen(StringToService->string), strlen("string"));
    // FreeStringToService(StringToService);
}

TEST(StringToService, FreeStringToService) {
    struct StringToService* StringToService = (struct StringToService*)malloc(sizeof(struct StringToService));
    char* string = (char*)"string";
    Service* service = (Service*)malloc(sizeof(Service));
    InitService(service, string);
    InitStringToService(StringToService, string, service);
    FreeStringToService(StringToService);
}

TEST(StringToService, AddNewService) {
    struct hashmap* StringToServiceMap = GetStringToServiceMap();
    char* serviceName = (char*)"serviceName";
    Service* service = AddNewService(StringToServiceMap, serviceName);
    EXPECT_EQ(strlen(service->serviceName), strlen("serviceName"));
    FreeStringToServiceMap(StringToServiceMap);
}

TEST(StringToService, FindAddedService) {
    struct hashmap* StringToServiceMap = GetStringToServiceMap();
    char* serviceName = (char*)"serviceName";
    Service* service = AddNewService(StringToServiceMap, serviceName);
    
    Service* foundService = FindService(StringToServiceMap, serviceName);

    EXPECT_EQ(strlen(foundService->serviceName), strlen("serviceName"));
    FreeStringToServiceMap(StringToServiceMap);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}