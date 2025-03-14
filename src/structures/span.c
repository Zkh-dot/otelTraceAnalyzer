#include "span.h"

const char* spanStatusMessage[TraceOk + 1] = {
    "UndefSpanStatus",
    "MissingParent",
    "NoParentInTrace",
    "DublicateSpan",
    "BadSpanIdSize",
    "SpanOk",
};

void InitSpan(
        Span* span,
        const char* spanId,
        const char* serviceName,
        const char* parentSpanId,
        Span* parentSpan
    ) {
    span->spanId = strdup(spanId);
    span->serviceName = strdup(serviceName);
    if(parentSpanId != NULL)
        span->parentSpanId = strdup(parentSpanId);
    else
        span->parentSpanId = NULL;
    span->parentSpan = parentSpan;
    span->spanStatus = UndefSpanStatus;
}

void FreeSpan(Span* span) {
    free(span->spanId);
    free(span->serviceName);
    if(span->parentSpanId != NULL)
        free(span->parentSpanId);
    free(span);
}

void FreeSpanTree(Span* span) {
    if (span->parentSpan != NULL) {
        FreeSpanTree(span->parentSpan);
    }
    FreeSpan(span);
}

void FreeAllSpans(Span** spans, int count) {
    for (int i = 0; i < count; i++) {
        FreeSpan(spans[i]);
    }
    free(spans);
}

Span* spancpy(Span* target, Span* source) {
    InitSpan(target, source->spanId, source->serviceName, source->parentSpanId, source->parentSpan);
    target->spanStatus = source->spanStatus;
    return target;
}

Span* FindSpan(Span** spans, int count, const char* spanId) {
    for (int i = 0; i < count; i++) {
        if (strcmp(spans[i]->spanId, spanId) == 0) {
            return spans[i];
        }
    }
    return NULL;
}

// not tested
void BuildSpanTree(Span** spans, int count) {
    Span* root = NULL;
    for (int i = 0; i < count; i++) {
        if (spans[i]->parentSpanId == NULL) {
            root = spans[i];
        } else {
            Span* parentSpan = FindSpan(spans, count, spans[i]->parentSpanId);
            if (parentSpan != NULL) {
                spans[i]->parentSpan = parentSpan;
            } else {
                spans[i]->spanStatus = NoParentInTrace;
            }
        }
    }
}

// not tested
int SpanTreeLength(Span* span) {
    int length = 0;
    while (span != NULL) {
        length++;
        span = span->parentSpan;
    }
    return length;
}
