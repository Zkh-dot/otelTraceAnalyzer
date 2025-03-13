#pragma once

#include "config.h"
#include <stdlib.h>

typedef enum {
    UndefSpanStatus,
    MissingParent,
    NoParentInTrace,
    DublicateSpan,
    BadSpanIdSize,
    SpanOk,
} SpanStatusTypes;

typedef enum {
    UndefTraceStatus,

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

void InitServiceErrorCounters(ServiceErrorCounters* counters);

void FreeServiceErrorCounters(ServiceErrorCounters* counters);

void sumCounters(ServiceErrorCounters* errorCounters, ServiceErrorCounters* errorCounters2);

void IncCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, char* traceId, bool isMy);

void DecCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, char* traceId, bool isMy);

void AppendExample(ServiceErrorCounters* errorCounters, char* traceId, bool isMy);

bool IsRootSpanError(ServiceErrorCounters* errorCounters);
