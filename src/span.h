#pragma once

#include "counters.h"
#include <string.h> 
#include <stdlib.h>

typedef struct Span_{
    char* spanId;
    char* serviceName;
    struct Span_* parentSpan;
    SpanStatusTypes spanStatus;
} Span;

Span* InitSpan(Span* span, char* spanId, char* serviceName, Span* parentSpan);

void FreeSpan(Span* span);

Span* spancpy(Span* target, Span* source);