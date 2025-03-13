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

int StringToServiceCompare(const void *a, const void *b, void *udata);

uint64_t StringToServiceHash(const void *item, uint64_t seed0, uint64_t seed1);

void InitStringToService(struct StringToService* stringToService, char* string, Service* service);

struct hashmap* GetStringToServiceMap();

Service* AddNewService(struct hashmap* stringToServiceMap, char* serviceName);

void FreeStringToService(struct StringToService* stringToService);

void FreeStringToServiceMap(struct hashmap* stringToServiceMap);

Service* FindService(struct hashmap* stringToServiceMap, char* serviceName);