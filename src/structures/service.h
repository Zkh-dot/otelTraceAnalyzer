#pragma once

#include "trace.h"
#include "counters.h"

typedef struct {
    char* serviceName;
    ServiceErrorCounters* errorCounters;
    Trace** traces;
} Service;

void InitService(Service* service, char* serviceName);

void FreeService(Service* service);