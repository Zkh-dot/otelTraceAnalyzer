#pragma once

#include "counters.h"
#include <string.h> 
#include <stdlib.h>


extern const char* spanStatusMessage[TraceOk + 1];

typedef struct Span_{
    char* spanId;
    char* serviceName;
    char* parentSpanId;
    char* traceId;
    struct Span_* parentSpan;
    SpanStatusTypes spanStatus;
} Span;

void InitSpan(
    Span* span,
    const char* spanId,
    const char* serviceName,
    const char* parentSpanId,
    Span* parentSpan
);

void FreeSpan(Span* span);

void FreeSpanTree(Span* span);

Span* spancpy(Span* target, Span* source);

void FreeAllSpans(Span** spans, int count);

Span* FindSpan(Span** spans, int count, const char* spanId);

void BuildSpanTree(Span** spans, int count);

int SpanTreeLength(Span* span);
