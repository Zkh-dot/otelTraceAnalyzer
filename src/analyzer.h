#pragma once

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
    bool storeTraces;
} Analyzer;

void InitAnalyzer(Analyzer* analyzer);

void FreeAnalyzer(Analyzer* analyzer);

void AddTrace(Analyzer* analyzer, Trace* trace);

Service* GetAddService(Analyzer* analyzer, const char* serviceName);

void ParceTrace(Analyzer* analyzer, Trace* trace);

void AnalyzeTrace(Analyzer* analyzer, Trace* trace);

void APIAnalyzeTrace(
    Analyzer* analyzer,
    const char* traceString,
    const char* serviceName,
    const char* traceId
);

ServiceErrorCounters* APIGetServiceErrorCounters(Analyzer* analyzer, const char* serviceName);

CountersArr* APIGetAllServiceErrorCounters(Analyzer* analyzer);

extern void (*pluginPtrs[])(Analyzer*, Trace*);
extern const int pluginCount;
