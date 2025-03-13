#include "counters.h"

ServiceErrorCounters* InitServiceErrorCounters(ServiceErrorCounters* counters) {
    for(int i = 0; i < TraceOk; i++) {
        counters->statusCounter[i] = 0;
    }
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        counters->myBadTraceExamples[i] = (char*)malloc(TRACE_ID_LENGTH);
        counters->myBadTraceExamples[i] = "";
        counters->notmyBadTraceExamples[i] = (char*)malloc(TRACE_ID_LENGTH);
        counters->notmyBadTraceExamples[i] = "";
    }
    counters->myExamplesCount = 0;
    counters->notmyExamplesCount = 0;
    return counters;
}

void FreeServiceErrorCounters(ServiceErrorCounters* counters) {
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        free(counters->myBadTraceExamples[i]);
        free(counters->notmyBadTraceExamples[i]);
    }
    free(counters);
}

void sumCounters(ServiceErrorCounters* errorCounters, ServiceErrorCounters* errorCounters2) {
    for(int i = 0; i < TraceOk; i++) {
        errorCounters->statusCounter[i] += errorCounters2->statusCounter[i];
    }
    for(int i = 0; i < EXAMPLES_LENGTH; i++) {
        if(errorCounters->myExamplesCount < EXAMPLES_LENGTH && errorCounters2->myExamplesCount > i) {
            errorCounters->myBadTraceExamples[errorCounters->myExamplesCount] = errorCounters2->myBadTraceExamples[i];
            errorCounters->myExamplesCount++;
        }
        if(errorCounters->notmyExamplesCount < EXAMPLES_LENGTH && errorCounters2->notmyExamplesCount > i) {
            errorCounters->notmyBadTraceExamples[errorCounters->notmyExamplesCount] = errorCounters2->notmyBadTraceExamples[i];
            errorCounters->notmyExamplesCount++;
        }
    }
}