#include "structures/counters.h"
#include "structures/span.h"
#include "structures/trace.h"
#include "structures/service.h"
#include "structures/servicemap.h"

typedef struct {
    struct hashmap* serviceMap;
    struct hashmap* traceMap;
    int serviceCount;
    int traceCount;
} Analyzer;

void InitAnalyzer(Analyzer* analyzer);

void FreeAnalyzer(Analyzer* analyzer);

void AddTrace(Analyzer* analyzer, Trace* trace);

Service* GetAddService(Analyzer* analyzer, char* serviceName);

void AnalyzeTrace(Analyzer* analyzer, Trace* trace);

void APIAnalyzeTrace(Analyzer* analyzer, char* traceString, char* serviceName, char* traceId);

ServiceErrorCounters* APIGetServiceErrorCounters(Analyzer* analyzer, char* serviceName);

CountersArr* APIGetAllServiceErrorCounters(Analyzer* analyzer);