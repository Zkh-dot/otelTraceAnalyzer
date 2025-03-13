#include <string>
#include "counters.h"

typedef struct Span{
    char* spanId;
    char* serviceName;
    Span* parentSpan;
    SpanStatusTypes spanStatus;
} Span;

Span* InitSpan(Span* span, char* spanId, char* serviceName, Span* parentSpan);

void FreeSpan(Span* span);

void FreeSpanTree(Span* span);

Span* spancpy(Span* target, Span* source);