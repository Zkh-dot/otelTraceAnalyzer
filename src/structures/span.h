#pragma once

#include "counters.h"
#include <string.h> 
#include <stdlib.h>


extern const char* spanStatusMessage[TraceOk + 1];

typedef struct Span_{
    char* spanId;
    char* serviceName;
    struct Span_* parentSpan;
    SpanStatusTypes spanStatus;
} Span;

void InitSpan(Span* span, char* spanId, char* serviceName, Span* parentSpan);

void FreeSpan(Span* span);

void FreeSpanTree(Span* span);

Span* spancpy(Span* target, Span* source);

void FreeAllSpans(Span** spans, int count);
