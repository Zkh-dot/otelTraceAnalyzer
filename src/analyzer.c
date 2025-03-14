#include "analyzer.h"

void InitAnalyzer(Analyzer* analyzer) {
    analyzer->serviceMap = GetStringToServiceMap();
    analyzer->traceMap = GetStringToTraceMap();
}

void FreeAnalyzer(Analyzer* analyzer) {
    FreeStringToServiceMap(analyzer->serviceMap);
    FreeStringToTraceMap(analyzer->traceMap);
}

void AddTrace(Analyzer* analyzer, Trace* trace) {
    struct StringToTrace* stringToTrace = (struct StringToTrace*)malloc(sizeof(struct StringToTrace));
    InitStringToTrace(stringToTrace, trace->traceId, trace);
    hashmap_set(analyzer->traceMap, stringToTrace);
    free(stringToTrace);
}

Service* GetAddService(Analyzer* analyzer, char* serviceName) {
    Service* tmpService = FindService(analyzer->serviceMap, serviceName);
    if(tmpService == NULL) {
        tmpService = AddNewService(analyzer->serviceMap, serviceName);
        analyzer->serviceCount++;
    }
    return tmpService;
}

void AnalyzeTrace(Analyzer* analyzer, Trace* trace) {
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
        if(trace->spans[i] == NULL) {
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
            free(notmyService);
        }
    }
    if(!IsRootSpanError(tmpCounters)) {
        sumCounters(myService->errorCounters, tmpCounters);
    }
    free(myService);
    FreeServiceErrorCounters(tmpCounters);
}

void APIAnalyzeTrace(Analyzer* analyzer, char* traceString, char* serviceName, char* traceId) {
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    InitTrace(trace, traceString, serviceName, traceId);
    AnalyzeTrace(analyzer, trace);
    free(trace);
}

ServiceErrorCounters* APIGetServiceErrorCounters(Analyzer* analyzer, char* serviceName) {
    Service* service = FindService(analyzer->serviceMap, serviceName);
    ServiceErrorCounters* counters = service->errorCounters;
    free(service);
    return counters;
}

CountersArr* APIGetAllServiceErrorCounters(Analyzer* analyzer) {
    CountersArr* arr = (CountersArr*)malloc(sizeof(CountersArr));
    ServiceErrorCounters** counters = (ServiceErrorCounters**)malloc(analyzer->serviceCount * sizeof(ServiceErrorCounters*));
    size_t iter = 0;
    void *item;
    while((hashmap_iter(analyzer->serviceMap, &iter, &item))) {
        struct StringToService* e = (struct StringToService*)item;
        counters[iter] = e->service->errorCounters;
        FreeTempStringToService(e);
    }
    for(int i = 0; i < iter; i++) {
        arr->errorCounters[i] = counters[i];
    }
    arr->errorCountersCount = iter;
    FreeTempStringToService((struct StringToService*)item);
    free(counters);
    return arr;
}
