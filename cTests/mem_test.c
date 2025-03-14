#include "../src/analyzer.h"

void analyze() {
    Analyzer* analyzer = (Analyzer*)malloc(sizeof(Analyzer));
    InitAnalyzer(analyzer);
    char* traceString = (char*)"[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    InitTrace(trace, traceString, serviceName, traceId);
    AnalyzeTrace(analyzer, trace);
    ServiceErrorCounters* counters = APIGetServiceErrorCounters(analyzer, serviceName);
    FreeAnalyzer(analyzer);
    // free(counters);
}

void servicemap() {
    struct hashmap* StringToServiceMap = GetStringToServiceMap();
    char* serviceName = (char*)"serviceName";
    Service* service = AddNewService(StringToServiceMap, serviceName);
    FreeStringToServiceMap(StringToServiceMap);
}

int main() {
    analyze();
}