#include "../src/analyzer.h"

int main() {
    Analyzer* analyzer = (Analyzer*)malloc(sizeof(Analyzer));
    InitAnalyzer(analyzer);
    char* traceString = (char*)"[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    APIAnalyzeTrace(analyzer, traceString, serviceName, traceId);
    CountersArr* countersArr = APIGetAllServiceErrorCounters(analyzer);
    FreeAnalyzer(analyzer);
    free(countersArr);
}