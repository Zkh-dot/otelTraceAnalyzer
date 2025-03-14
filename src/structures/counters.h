#pragma once

#include "config.h"
#include <stdlib.h>
#include <stdbool.h>

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
    int statusCounter[TraceOk + 1];
    char* myBadTraceExamples[EXAMPLES_LENGTH];
    char* notmyBadTraceExamples[EXAMPLES_LENGTH];
    int myExamplesCount;
    int notmyExamplesCount;
    int badTraceCount;
    int mySpanCount;
    int traceCount;
} ServiceErrorCounters;

typedef struct {
    ServiceErrorCounters** errorCounters;
    int errorCountersCount;
} CountersArr;

void InitServiceErrorCounters(ServiceErrorCounters* counters);

void FreeServiceErrorCounters(ServiceErrorCounters* counters);

void sumCounters(ServiceErrorCounters* errorCounters, ServiceErrorCounters* errorCounters2);

void IncCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, bool isMy);

void DecCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, bool isMy);

void AppendExample(ServiceErrorCounters* errorCounters, char* traceId, bool isMy);

bool IsRootSpanError(ServiceErrorCounters* errorCounters);

void FreeCountersArr(CountersArr* countersArr);

void InitCountersArr(CountersArr* countersArr, int size);