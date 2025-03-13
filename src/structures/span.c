#include "span.h"

const char* spanStatusMessage[TraceOk + 1] = {
    "UndefSpanStatus",
    "MissingParent",
    "NoParentInTrace",
    "DublicateSpan",
    "BadSpanIdSize",
    "SpanOk",
};

void InitSpan(Span* span, char* spanId, char* serviceName, Span* parentSpan) {
    span->spanId = (char*)malloc(strlen(spanId) + 1);
    strcpy(span->spanId, spanId);
    span->serviceName = (char*)malloc(strlen(serviceName) + 1);
    strcpy(span->serviceName, serviceName);
    span->parentSpan = parentSpan;
    span->spanStatus = UndefSpanStatus;
}

void FreeSpan(Span* span) {
    free(span->spanId);
    free(span->serviceName);
    // free(span->parentSpan);
    free(span);
}

void FreeSpanTree(Span* span) {
    if (span->parentSpan != NULL) {
        FreeSpanTree(span->parentSpan);
    }
    FreeSpan(span);
}

Span* spancpy(Span* target, Span* source) {
    InitSpan(target, source->spanId, source->serviceName, source->parentSpan);
    target->spanStatus = source->spanStatus;
    return target;
}