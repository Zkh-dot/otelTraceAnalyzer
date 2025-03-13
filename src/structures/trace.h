#pragma once

#include "../hashstructs/hashset.h"
#include "../hashstructs/hashset_itr.h"
#include "span.h"
#include "counters.h" 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern const char* traceStatusMessage[TraceOk + 1];

typedef struct {
    char* traceString;
    char* serviceName;
    char* traceId;
    hashset_t spanIds;
} Trace;

char *LongStrTok(char *input, const char *delimiter);

int CountSpans(char* trace);

char* ScanTrace(const char *field, const char *trace);

void InitTrace(Trace* trace, char* traceString, char* serviceName, char* traceId);

Span** FindAllSpans(Trace* trace);

void FreeTrace(Trace* trace);