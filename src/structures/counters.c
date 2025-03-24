#include "counters.h"

void InitServiceErrorCounters(ServiceErrorCounters* counters) {
    for(int i = 0; i < TraceOk + 1; i++) {
        counters->statusCounter[i] = 0;
    }
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        counters->myBadTraceExamples[i] = "";
        counters->notmyBadTraceExamples[i] = "";
    }
    counters->myExamplesCount = 0;
    counters->notmyExamplesCount = 0;
    counters->badTraceCount = 0;
    counters->mySpanCount = 0;
    counters->traceCount = 0;
}

void FreeServiceErrorCounters(ServiceErrorCounters* counters) {
    free(counters);
}

void sumCounters(ServiceErrorCounters* dst, ServiceErrorCounters* src) {
    for(int i = 0; i < TraceOk; i++) {
        dst->statusCounter[i] += src->statusCounter[i];
    }
}

void IncCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, bool isMy) {
    switch (status)
    {
    case MissingParent:
        errorCounters->statusCounter[myMissingParent + !isMy]++;
        break;
    case NoParentInTrace:
        errorCounters->statusCounter[myNoParentInTrace + !isMy]++;
        break;
    case DublicateSpan: 
        errorCounters->statusCounter[myDublicateSpan + !isMy]++;
        break;
    case BadSpanIdSize:
        errorCounters->statusCounter[myBadSpanIdSize + !isMy]++;
        break;
    case SpanOk:
        errorCounters->statusCounter[TraceOk]++;
    default:
        break;
    }
}

void DecCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, bool isMy) {
    switch (status)
    {
    case MissingParent:
        errorCounters->statusCounter[myMissingParent + !isMy]--;
        break;
    case NoParentInTrace:
        errorCounters->statusCounter[myNoParentInTrace + !isMy]--;
        break;
    case DublicateSpan: 
        errorCounters->statusCounter[myDublicateSpan + !isMy]--;
        break;
    case BadSpanIdSize:
        errorCounters->statusCounter[myBadSpanIdSize + !isMy]--;
        break;
    case SpanOk:
        errorCounters->statusCounter[TraceOk]--;
    default:
        break;
    }
}

void AppendExample(ServiceErrorCounters* errorCounters, char* traceId, bool isMy) {
    if(isMy) {
        if(errorCounters->myExamplesCount < EXAMPLES_LENGTH) {
            errorCounters->myBadTraceExamples[errorCounters->myExamplesCount] = traceId;
            errorCounters->myExamplesCount++;
        }
    } else {
        if(errorCounters->notmyExamplesCount < EXAMPLES_LENGTH) {
            errorCounters->notmyBadTraceExamples[errorCounters->notmyExamplesCount] = traceId;
            errorCounters->notmyExamplesCount++;
        }
    }
}

bool IsRootSpanError(ServiceErrorCounters* errorCounters) {
    int changed = 0;
    for(int i = 0; i < TraceOk; i++) {
        changed += errorCounters->statusCounter[i];
    }
    return ((changed == 1) && errorCounters->statusCounter[myMissingParent] == 1) || changed == 0;
}

void FreeCountersArr(CountersArr* countersArr) {
    for(int i = 0; i < countersArr->errorCountersCount; i++) {
        FreeServiceErrorCounters(countersArr->errorCounters[i]);
    }
    free(countersArr);
}
