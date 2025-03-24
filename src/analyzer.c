#include "analyzer.h"

void InitAnalyzer(Analyzer* analyzer) {
    analyzer->serviceMap = GetStringToServiceMap();
    analyzer->traceMap = GetStringToTraceMap();
    analyzer->serviceCount = 0;
    analyzer->traceCount = 0;
    analyzer->storeTraces = false;
}

void FreeAnalyzer(Analyzer* analyzer) {
    FreeStringToServiceMap(analyzer->serviceMap);
    FreeStringToTraceMap(analyzer->traceMap);
    free(analyzer);
}

void AddTrace(Analyzer* analyzer, Trace* trace) {
    struct StringToTrace* stringToTrace = (struct StringToTrace*)malloc(sizeof(struct StringToTrace));
    InitStringToTrace(stringToTrace, trace->traceId, trace);
    hashmap_set(analyzer->traceMap, stringToTrace);
    free(stringToTrace);
}

Service* GetAddService(Analyzer* analyzer, const char* serviceName) {
    Service* tmpService = FindService(analyzer->serviceMap, serviceName);
    if(tmpService == NULL) {
        tmpService = AddNewService(analyzer->serviceMap, serviceName);
        analyzer->serviceCount++;
    }
    return tmpService;
}

void ParceTrace(Analyzer* analyzer, Trace* trace) {
    if(analyzer->storeTraces)
        AddTrace(analyzer, trace);
    FindAllSpans(trace);
    int badSpanCount = 0;
    Service* myService = GetAddService(analyzer, trace->serviceName);
    ServiceErrorCounters* tmpCounters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    myService->errorCounters->traceCount++;
    InitServiceErrorCounters(tmpCounters);
    for(int i = 0; i < trace->spansCount; i++) {
        bool isMy = strcmp(trace->serviceName, trace->spans[i]->serviceName) == 0;
        myService->errorCounters->mySpanCount += isMy;
        if(trace->spans[i]->parentSpanId == NULL) {
            trace->spans[i]->spanStatus = MissingParent;
            badSpanCount++;
        } else {
            if(!hashset_is_member(trace->spanIds, trace->spans[i]->parentSpanId)) {
                trace->spans[i]->spanStatus = NoParentInTrace;
                badSpanCount++;
            }
            if(strlen(trace->spans[i]->spanId) != SPAN_ID_LENGTH) {
                trace->spans[i]->spanStatus = BadSpanIdSize;
                badSpanCount++;
            }
        }
        if(trace->spans[i]->spanStatus == SpanOk) {
            continue;
        }
        if(isMy) {
            IncCounters(tmpCounters, trace->spans[i]->spanStatus, isMy);
            AppendExample(tmpCounters, trace->traceId, isMy);
            tmpCounters->badTraceCount++;
        } else {
            Service* notmyService = GetAddService(analyzer, trace->spans[i]->serviceName);
            IncCounters(notmyService->errorCounters, trace->spans[i]->spanStatus, isMy);
            AppendExample(notmyService->errorCounters, trace->traceId, isMy);
        }
    }
    if(!IsRootSpanError(tmpCounters)) {
        sumCounters(myService->errorCounters, tmpCounters);
        AppendExample(myService->errorCounters, trace->traceId, 1);
    } else {
            IncCounters(myService->errorCounters, SpanOk, true);
    }
    free(tmpCounters);
}

void RunPlugins(Analyzer* analyzer, Trace* trace) {
    for(int i = 0; i < pluginCount; i++) {
        pluginPtrs[i](analyzer, trace);
    }
}

void APIAnalyzeTraceObj(Analyzer* analyzer, Trace* trace) {
    ParceTrace(analyzer, trace);

    RunPlugins(analyzer, trace);

    if(!analyzer->storeTraces)
        FreeTrace(trace);
}

void APIAnalyzeTrace(
        Analyzer* analyzer,
        const char* traceString,
        const char* serviceName,
        const char* traceId
    ) {
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    InitTrace(trace, traceString, serviceName, traceId);
    APIAnalyzeTraceObj(analyzer, trace);
}

ServiceErrorCounters* APIGetServiceErrorCounters(Analyzer* analyzer, const char* serviceName) {
    Service* service = FindService(analyzer->serviceMap, serviceName);
    ServiceErrorCounters* counters = service->errorCounters;
    return counters;
}

CountersArr* APIGetAllServiceErrorCounters(Analyzer* analyzer) {
    CountersArr* arr = (CountersArr*)malloc(sizeof(CountersArr));
    ServiceErrorCounters** counters = (ServiceErrorCounters**)malloc(analyzer->serviceCount * sizeof(ServiceErrorCounters*));
    size_t iter = 0;
    int counter = 0;
    void *item;
    struct StringToService* e;
    while((hashmap_iter(analyzer->serviceMap, &iter, &item))) {
        e = (struct StringToService*)item;
        counters[counter] = e->service->errorCounters;
        counters[counter]->serviceName = e->string;
        counter++;
    }
    arr->errorCounters = counters;
    arr->errorCountersCount = counter;
    return arr;
}
