#include "counters.h"

void InitServiceErrorCounters(ServiceErrorCounters* counters) {
    for(int i = 0; i < TraceOk + 1; i++) {
        counters->statusCounter[i] = 0;
    }
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        counters->myBadTraceExamples[i] = NULL;
        counters->notmyBadTraceExamples[i] = NULL;
    }
    counters->myExamplesCount = 0;
    counters->notmyExamplesCount = 0;
    counters->badTraceCount = 0;
    counters->mySpanCount = 0;
    counters->notmySpanCount = 0;
    counters->traceCount = 0;
    counters->inTraceSpanCount = 0;
    counters->serviceName = NULL;
}

void FreeServiceErrorCounters(ServiceErrorCounters* counters) {
    for(int i = 0; i < counters->myExamplesCount; i++) {
        if(counters->myBadTraceExamples[i] != NULL)
            free(counters->myBadTraceExamples[i]);
    }
    for(int i = 0; i < counters->notmyExamplesCount; i++) {
        if(counters->notmyBadTraceExamples[i] != NULL)
            free(counters->notmyBadTraceExamples[i]);
    }
    // if(counters->serviceName != NULL) {
    //     free(counters->serviceName);
    // }
    free(counters);
}

void sumCounters(ServiceErrorCounters* dst, ServiceErrorCounters* src) {
    for(int i = 0; i < TraceOk + 1; i++) {
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
    case NoServivceName:
        errorCounters->statusCounter[noServiceNameSpan]++;
        break;
    case SpanOk:
        errorCounters->statusCounter[TraceOk]++;
    default:
        break;
    }
}

void DecCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, bool isMy) {
    // TODO: it can be better, we do not diffirentiate between reason of example
    free(errorCounters->myBadTraceExamples[--errorCounters->myExamplesCount]);
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
    case NoServivceName:
        errorCounters->statusCounter[noServiceNameSpan]--;
        break;
    case SpanOk:
        errorCounters->statusCounter[TraceOk]--;
    default:
        break;
    }
}

void AppendExample(ServiceErrorCounters* errorCounters, const char* traceId, bool isMy) {
    if(isMy) {
        if(errorCounters->myExamplesCount < EXAMPLES_LENGTH) {
            errorCounters->myBadTraceExamples[errorCounters->myExamplesCount] = strdup(traceId);
            errorCounters->myExamplesCount++;
        }
    } else {
        if(errorCounters->notmyExamplesCount < EXAMPLES_LENGTH) {
            errorCounters->notmyBadTraceExamples[errorCounters->notmyExamplesCount] = strdup(traceId);
            errorCounters->notmyExamplesCount++;
        }
    }
}

bool IsRootSpanError(ServiceErrorCounters* errorCounters) {
    int changed = 0;
    for(int i = 0; i < TraceOk + 1; i++) {
        changed += errorCounters->statusCounter[i];
    }
    return (changed == 1) && errorCounters->statusCounter[myMissingParent] == 1;
}

void FreeCountersArr(CountersArr* countersArr) {
    for(int i = 0; i < countersArr->errorCountersCount; i++) {
        FreeServiceErrorCounters(countersArr->errorCounters[i]);
    }
    free(countersArr);
}

void countercpy(ServiceErrorCounters* dst, ServiceErrorCounters* src) {
    for(int i = 0; i < TraceOk + 1; i++) {
        dst->statusCounter[i] = src->statusCounter[i];
    }
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        if(src->myBadTraceExamples[i] != NULL)
            dst->myBadTraceExamples[i] = strdup(src->myBadTraceExamples[i]);
        if(src->notmyBadTraceExamples[i] != NULL)
            dst->notmyBadTraceExamples[i] = strdup(src->notmyBadTraceExamples[i]);
    }
    dst->myExamplesCount = src->myExamplesCount;
    dst->notmyExamplesCount = src->notmyExamplesCount;
    dst->badTraceCount = src->badTraceCount;
    dst->mySpanCount = src->mySpanCount;
    dst->notmySpanCount = src->notmySpanCount;
    dst->inTraceSpanCount = src->inTraceSpanCount;
    dst->traceCount = src->traceCount;
}