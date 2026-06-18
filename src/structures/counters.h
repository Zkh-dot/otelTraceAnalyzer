#pragma once

#include "config.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    UndefSpanStatus,
    MissingParent,
    NoParentInTrace,
    DublicateSpan,
    BadSpanIdSize,
    NoServiceName,
    SelfParent,
    SpanOk,
} SpanStatusTypes;

typedef enum {
    UndefTraceStatus,

    noServiceNameSpan,

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
    char* myBadTraceExamples[EXAMPLES_LENGTH];
    char* notmyBadTraceExamples[EXAMPLES_LENGTH];
    int myExamplesCount;
    int notmyExamplesCount;
    int mySpanCount;
    int notmySpanCount;
} ServiceCounters;

typedef struct {
    char* serviceName;
    int myTraceStatusCounter[TraceOk + 1];
    int notmyTraceStatusCounter[TraceOk + 1];
    int traceCount;
    int inTraceSpanCount;
    int badTraceCount;
    ServiceCounters myTraceServiceCounters;
    ServiceCounters notmyTraceServiceCounters;
} ServiceErrorCounters;

typedef struct {
    ServiceErrorCounters** errorCounters;
    int errorCountersCount;
    char* serviceName;
} CountersArr;

void InitServiceCounters(ServiceCounters* counters);

void FreeServiceCounters(ServiceCounters* counters);

void InitServiceErrorCounters(ServiceErrorCounters* counters);

void FreeServiceErrorCounters(ServiceErrorCounters* counters);

static void CopyServiceCounters(ServiceCounters* dst, ServiceCounters* src);

void CopyServiceErrorCounters(ServiceErrorCounters* dst, ServiceErrorCounters* src);

void sumCounters(ServiceErrorCounters* dst, ServiceErrorCounters* src);

void IncCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, bool isMyTrace, bool isMySpan);

void DecCounters(ServiceErrorCounters* errorCounters, SpanStatusTypes status, bool isMyTrace, bool isMySpan);

void AppendExample(ServiceErrorCounters* errorCounters, const char* traceId, bool isMyTrace, bool isMySpan);

void FreeCountersArr(CountersArr* countersArr);
