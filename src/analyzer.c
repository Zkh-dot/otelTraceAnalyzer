#include "analyzer.h"
#include <string.h>

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
    struct StringToTrace stringToTrace;
    InitStringToTrace(&stringToTrace, trace->traceId, trace);
    hashmap_set(analyzer->traceMap, &stringToTrace);
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
    myService->errorCounters->traceCount++;
    myService->errorCounters->inTraceSpanCount += trace->spansCount;
    bool haveSeenRootSpan = false;
    for(int i = 0; i < trace->spansCount; i++) {
        if(trace->spans[i]->spanStatus == NoServiceName) {
            IncCounters(myService->errorCounters, NoServiceName, true, true);
            AppendExample(myService->errorCounters, trace->traceId, true, true);
            trace->goodTrace = false;
            continue;
        }
        bool isMy = strcmp(trace->serviceName, trace->spans[i]->serviceName) == 0;
        myService->errorCounters->myTraceServiceCounters.mySpanCount += isMy;
        myService->errorCounters->myTraceServiceCounters.notmySpanCount += !isMy;

        if(trace->spans[i]->parentSpanId == NULL) {
            if(!haveSeenRootSpan) {
                trace->spans[i]->spanStatus = SpanOk;
                haveSeenRootSpan = true;
            } else {
                trace->spans[i]->spanStatus = MissingParent;
                badSpanCount++;
            }
        } else {
            if(FindSpan(trace->spans, trace->spansCount, trace->spans[i]->parentSpanId) == NULL) {
                trace->spans[i]->spanStatus = NoParentInTrace;
                badSpanCount++;
            }
            if(strlen(trace->spans[i]->spanId) != SPAN_ID_LENGTH) {
                trace->spans[i]->spanStatus = BadSpanIdSize;
                badSpanCount++;
            }
            if(strcmp(trace->spans[i]->parentSpanId, trace->spans[i]->spanId) == 0) {
                trace->spans[i]->spanStatus = SelfParent;
                badSpanCount++;
            }
        }
        if(trace->spans[i]->spanStatus == UndefSpanStatus) {
            trace->spans[i]->spanStatus = SpanOk;
        }
        if(isMy) {
            IncCounters(myService->errorCounters, trace->spans[i]->spanStatus, true, true);
            if(trace->spans[i]->spanStatus != SpanOk)
                AppendExample(myService->errorCounters, trace->traceId, true, true);
        } else {
            Service* notmyService = GetAddService(analyzer, trace->spans[i]->serviceName);
            notmyService->errorCounters->notmyTraceServiceCounters.mySpanCount++;
            IncCounters(notmyService->errorCounters, trace->spans[i]->spanStatus,false, true);
            IncCounters(myService->errorCounters, trace->spans[i]->spanStatus, true, false);
            if(trace->spans[i]->spanStatus != SpanOk) {
                AppendExample(notmyService->errorCounters, trace->traceId, false, true);
                AppendExample(myService->errorCounters, trace->traceId, true, false);
            }
        }
        trace->goodTrace = trace->goodTrace * (trace->spans[i]->spanStatus == SpanOk);
    }
    myService->errorCounters->badTraceCount += !trace->goodTrace;
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
    Service* service = GetAddService(analyzer, serviceName);
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
        counters[counter] = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
        InitServiceErrorCounters(counters[counter]);
        CopyServiceErrorCounters(counters[counter], e->service->errorCounters);
        counters[counter]->serviceName = strdup(e->string);
        counter++;
    }
    arr->errorCounters = counters;
    arr->errorCountersCount = counter;
    return arr;
}
