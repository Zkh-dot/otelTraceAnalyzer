#pragma once

#include "config.h"
#include "service.h"
#include "span.h"
#include "trace.h"
#include "../hashstructs/hashmap.h"

struct StringToService {
    char* string;
    Service* service;
};

struct StringToTrace {
    char* string;
    Trace* trace;
};

int StringToStructCompare(const void *a, const void *b, void *udata);

uint64_t StringToStructHash(const void *item, uint64_t seed0, uint64_t seed1);

void InitStringToService(struct StringToService* stringToService, char* string, Service* service);

void InitStringToTrace(struct StringToTrace* stringToTrace, char* string, Trace* trace);

struct hashmap* GetStringToServiceMap();

struct hashmap* GetStringToTraceMap();

Service* AddNewService(struct hashmap* stringToServiceMap, char* serviceName);

void FreeStringToService(struct StringToService* stringToService);

void FreeStringToTrace(struct StringToTrace* stringToTrace);

void FreeTempStringToService(struct StringToService* stringToService);

void FreeTempStringToTrace(struct StringToTrace* stringToTrace);

void FreeStringToServiceMap(struct hashmap* stringToServiceMap);

void FreeStringToTraceMap(struct hashmap* stringToTraceMap);

Service* FindService(struct hashmap* stringToServiceMap, char* serviceName);

Trace* FindTrace(struct hashmap* stringToTraceMap, char* traceId);