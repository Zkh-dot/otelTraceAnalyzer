#include "counters.h"

ServiceErrorCounters* InitServiceErrorCounters(ServiceErrorCounters* counters) {
    for(int i = 0; i < TraceOk; i++) {
        counters->statusCounter[i] = 0;
    }
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        counters->myBadTraceExamples[i] = "";
        counters->notmyBadTraceExamples[i] = "";
    }
    counters->myExamplesCount = 0;
    counters->notmyExamplesCount = 0;
    return counters;
}

void FreeServiceErrorCounters(ServiceErrorCounters* counters) {
    free(counters);
}

void sumCounters(ServiceErrorCounters* errorCounters, ServiceErrorCounters* errorCounters2) {
    for(int i = 0; i < TraceOk; i++) {
        errorCounters->statusCounter[i] += errorCounters2->statusCounter[i];
    }
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        AppendExample(errorCounters, errorCounters2->myBadTraceExamples[i], 1);
        AppendExample(errorCounters, errorCounters2->notmyBadTraceExamples[i], 0);
    }
}

void IncCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, char* traceId, bool isMy) {
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

void DecCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, char* traceId, bool isMy) {
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
    return (changed == 1) && errorCounters->statusCounter[myMissingParent] == 1;
}