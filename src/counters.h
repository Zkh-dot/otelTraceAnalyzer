#pragma once

#include "config.h"
#include <stdlib.h>

typedef enum {
    MissingParent,
    NoParentInTrace,
    DublicateSpan,
    BadSpanIdSize,
    SpanOk,
} SpanStatusTypes;

typedef enum {
    myMissingParent,
    notmyMissingParent,

    myNoParentInTrace,
    notmyNoParentInTrace,

    myDublicateSpan,
    notmyDublicateSpan,

    myBadSpanIdSize,
    notmyBadSpanIdSize,

    myBadTraceIdSize,
    notmyBadTraceIdSize,
    TraceOk,
} TraceStatusTypes;

typedef struct {
    int statusCounter[TraceOk];
    char* myBadTraceExamples[EXAMPLES_LENGTH];
    char* notmyBadTraceExamples[EXAMPLES_LENGTH];
    int myExamplesCount;
    int notmyExamplesCount;
} ServiceErrorCounters;

ServiceErrorCounters* InitServiceErrorCounters(ServiceErrorCounters* counters);

void FreeServiceErrorCounters(ServiceErrorCounters* counters);

void sumCounters(ServiceErrorCounters* errorCounters, ServiceErrorCounters* errorCounters2);

