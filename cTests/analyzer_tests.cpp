#include <gtest/gtest.h>
extern "C" {
    #include "../src/analyzer.h"
}

TEST(Analyzer, InitFreeAnalyzer) {
    Analyzer* analyzer = (Analyzer*)malloc(sizeof(Analyzer));
    InitAnalyzer(analyzer);

    FreeAnalyzer(analyzer);
}

TEST(Analyzer, AnalyzeTrace) {
    Analyzer* analyzer = (Analyzer*)malloc(sizeof(Analyzer));
    InitAnalyzer(analyzer);
    char* traceString = (char*)"[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    InitTrace(trace, traceString, serviceName, traceId);
    AnalyzeTrace(analyzer, trace);
    ServiceErrorCounters* counters = APIGetServiceErrorCounters(analyzer, serviceName);
    EXPECT_EQ(counters->traceCount, 1);
    FreeAnalyzer(analyzer);
    free(counters);
}

TEST(API, APIAnalyzeTrace) {
    Analyzer* analyzer = (Analyzer*)malloc(sizeof(Analyzer));
    InitAnalyzer(analyzer);
    char* traceString = (char*)"[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    APIAnalyzeTrace(analyzer, traceString, serviceName, traceId);
    ServiceErrorCounters* counters = APIGetServiceErrorCounters(analyzer, serviceName);
    EXPECT_EQ(counters->traceCount, 1);
    FreeAnalyzer(analyzer);
    free(counters);
}

TEST(API, APIGetServiceErrorCounters) {
    Analyzer* analyzer = (Analyzer*)malloc(sizeof(Analyzer));
    InitAnalyzer(analyzer);
    char* traceString = (char*)"[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    APIAnalyzeTrace(analyzer, traceString, serviceName, traceId);
    CountersArr* countersArr = APIGetAllServiceErrorCounters(analyzer);
    EXPECT_EQ(countersArr->errorCountersCount, 1);
    EXPECT_EQ(countersArr->errorCounters[0]->traceCount, 1);
    FreeAnalyzer(analyzer);
    free(countersArr);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}