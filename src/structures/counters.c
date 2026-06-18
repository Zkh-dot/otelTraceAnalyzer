#include "counters.h"
#include <string.h>

void InitServiceCounters(ServiceCounters* counters) {
    counters->myExamplesCount = 0;
    counters->notmyExamplesCount = 0;
    counters->mySpanCount = 0;
    counters->notmySpanCount = 0;
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        counters->myBadTraceExamples[i] = NULL;
        counters->notmyBadTraceExamples[i] = NULL;
    }
}

void FreeServiceCounters(ServiceCounters* counters) {
    for(int i = 0; i < counters->myExamplesCount; i++) {
        if(counters->myBadTraceExamples[i] != NULL)
            free(counters->myBadTraceExamples[i]);
    }
    for(int i = 0; i < counters->notmyExamplesCount; i++) {
        if(counters->notmyBadTraceExamples[i] != NULL)
            free(counters->notmyBadTraceExamples[i]);
    }
}

void InitServiceErrorCounters(ServiceErrorCounters* counters) {
    for(int i = 0; i < TraceOk + 1; i++) {
        counters->myTraceStatusCounter[i] = 0;
        counters->notmyTraceStatusCounter[i] = 0;
    }
    counters->serviceName = NULL;
    counters->traceCount = 0;
    counters->inTraceSpanCount = 0;
    counters->badTraceCount = 0;
    InitServiceCounters(&counters->myTraceServiceCounters);
    InitServiceCounters(&counters->notmyTraceServiceCounters);
}

void FreeServiceErrorCounters(ServiceErrorCounters* counters) {
    if(counters->serviceName != NULL)
        free(counters->serviceName);
    FreeServiceCounters(&counters->myTraceServiceCounters);
    FreeServiceCounters(&counters->notmyTraceServiceCounters);
    free(counters);
}

static void CopyServiceCounters(ServiceCounters* dst, ServiceCounters* src) {
    for(int i = 0; i < src->myExamplesCount; i++) {
        if(src->myBadTraceExamples[i] != NULL)
            dst->myBadTraceExamples[i] = strdup(src->myBadTraceExamples[i]);
    }
    for(int i = 0; i < src->notmyExamplesCount; i++) {
        if(src->notmyBadTraceExamples[i] != NULL)
            dst->notmyBadTraceExamples[i] = strdup(src->notmyBadTraceExamples[i]);
    }
    dst->myExamplesCount = src->myExamplesCount;
    dst->notmyExamplesCount = src->notmyExamplesCount;
    dst->mySpanCount = src->mySpanCount;
    dst->notmySpanCount = src->notmySpanCount;
}

void CopyServiceErrorCounters(ServiceErrorCounters* dst, ServiceErrorCounters* src) {
    for(int i = 0; i < TraceOk + 1; i++) {
        dst->myTraceStatusCounter[i] = src->myTraceStatusCounter[i];
        dst->notmyTraceStatusCounter[i] = src->notmyTraceStatusCounter[i];
    }
    if(src->serviceName != NULL)
        dst->serviceName = strdup(src->serviceName);
    dst->traceCount = src->traceCount;
    dst->inTraceSpanCount = src->inTraceSpanCount;
    dst->badTraceCount = src->badTraceCount;
    CopyServiceCounters(&dst->myTraceServiceCounters, &src->myTraceServiceCounters);
    CopyServiceCounters(&dst->notmyTraceServiceCounters, &src->notmyTraceServiceCounters);
}

void sumCounters(ServiceErrorCounters* dst, ServiceErrorCounters* src) {
    for(int i = 0; i < TraceOk + 1; i++) {
        dst->myTraceStatusCounter[i] += src->myTraceStatusCounter[i];
        dst->notmyTraceStatusCounter[i] += src->notmyTraceStatusCounter[i];
    }
}

void IncCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, bool isMyTrace, bool isMySpan) {
    int* counterPointer = isMyTrace ? errorCounters->myTraceStatusCounter : errorCounters->notmyTraceStatusCounter;
    switch (status)
    {
    case MissingParent:
        counterPointer[myMissingParent + !isMySpan]++;
        break;
    case NoParentInTrace:
        counterPointer[myNoParentInTrace + !isMySpan]++;
        break;
    case DublicateSpan: 
        counterPointer[myDublicateSpan + !isMySpan]++;
        break;
    case BadSpanIdSize:
        counterPointer[myBadSpanIdSize + !isMySpan]++;
        break;
    case NoServiceName:
        counterPointer[noServiceNameSpan]++;
        break;
    case SpanOk:
        counterPointer[TraceOk]++;
        break;
    case SelfParent:
        counterPointer[myMissingParent + !isMySpan]++;
        break;
    default:
        break;
    }
}

void DecCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, bool isMyTrace, bool isMySpan) {
    int* counterPointer = isMyTrace ? errorCounters->myTraceStatusCounter : errorCounters->notmyTraceStatusCounter;
    switch (status)
    {
    case MissingParent:
        counterPointer[myMissingParent + !isMySpan]--;
        break;
    case NoParentInTrace:
        counterPointer[myNoParentInTrace + !isMySpan]--;
        break;
    case DublicateSpan: 
        counterPointer[myDublicateSpan + !isMySpan]--;
        break;
    case BadSpanIdSize:
        counterPointer[myBadSpanIdSize + !isMySpan]--;
        break;
    case NoServiceName:
        counterPointer[noServiceNameSpan]--;
        break;
    case SpanOk:
        counterPointer[TraceOk]--;
        break;
    case SelfParent:
        counterPointer[myMissingParent + !isMySpan]--;
        break;
    default:
        break;
    }
}

void AppendExample(ServiceErrorCounters* errorCounters, const char* traceId, bool isMyTrace, bool isMySpan) {
    ServiceCounters* serviceCounterPointer = isMyTrace ? &errorCounters->myTraceServiceCounters : &errorCounters->notmyTraceServiceCounters;
    int* examplesCount = isMySpan ? &serviceCounterPointer->myExamplesCount : &serviceCounterPointer->notmyExamplesCount;
    char** badTraceExamples = isMySpan ? serviceCounterPointer->myBadTraceExamples : serviceCounterPointer->notmyBadTraceExamples;
    if(
        *examplesCount < EXAMPLES_LENGTH
        && (*examplesCount == 0 ? true : strcmp(badTraceExamples[*examplesCount - 1], traceId) != 0)
    ) {
        badTraceExamples[*examplesCount] = strdup(traceId);
        (*examplesCount)++;
    }
}

void FreeCountersArr(CountersArr* countersArr) {
    for(int i = 0; i < countersArr->errorCountersCount; i++) {
        FreeServiceErrorCounters(countersArr->errorCounters[i]);
    }
    free(countersArr->errorCounters);
    free(countersArr);
}
