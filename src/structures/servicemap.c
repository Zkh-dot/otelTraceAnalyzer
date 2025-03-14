#include "servicemap.h"

int StringToStructCompare(const void *a, const void *b, void *udata) {
    const struct StringToService *ua = a;
    const struct StringToService *ub = b;
    return strcmp(ua->string, ub->string);
};

uint64_t StringToStructHash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct StringToService *StringToInt = item;
    return hashmap_sip(StringToInt->string, strlen(StringToInt->string), seed0, seed1);
}

void InitStringToService(struct StringToService* stringToService, const char* string, Service* service) {
    stringToService->string = strdup(string);
    stringToService->service = service;
}

void InitStringToTrace(struct StringToTrace* stringToTrace, const char* string, Trace* trace) {
    stringToTrace->string = strdup(string);
    stringToTrace->trace = trace;
}

struct hashmap* GetStringToServiceMap() {
    return hashmap_new(sizeof(struct StringToService), 0, 0, 0, 
        StringToStructHash, StringToStructCompare, NULL, NULL);
}

struct hashmap* GetStringToTraceMap() {
    return hashmap_new(sizeof(struct StringToTrace), 0, 0, 0, 
        StringToStructHash, StringToStructCompare, NULL, NULL);
}

Service* AddNewService(struct hashmap* stringToServiceMap, const char* serviceName) {
    struct StringToService stringToService;
    Service* service = (Service*)malloc(sizeof(Service));
    InitService(service, serviceName);
    InitStringToService(&stringToService, serviceName, service);
    hashmap_set(stringToServiceMap, &stringToService);
    return service;
}

void FreeStringToService(struct StringToService* stringToService) {
    free(stringToService->string);
    FreeService(stringToService->service);
}

void FreeStringToTrace(struct StringToTrace* stringToTrace) {
    free(stringToTrace->string);
    FreeTrace(stringToTrace->trace);
}

void FreeTempStringToService(struct StringToService* stringToService) {
    free(stringToService->string);
    free(stringToService);
}

void FreeTempStringToTrace(struct StringToTrace* stringToTrace) {
    free(stringToTrace->string);
    free(stringToTrace);
}

void FreeStringToServiceMap(struct hashmap* stringToServiceMap) {
    size_t iter = 0;
    void *item;
    while (hashmap_iter(stringToServiceMap, &iter, &item))  {
        FreeStringToService((struct StringToService*)item);
    }
    hashmap_free(stringToServiceMap);
}

void FreeStringToTraceMap(struct hashmap* stringToTraceMap) {
    size_t iter = 0;
    void *item;
    while (hashmap_iter(stringToTraceMap, &iter, &item))  {
        FreeStringToTrace((struct StringToTrace*)item);
    }
    hashmap_free(stringToTraceMap);
}

Service* FindService(struct hashmap* stringToServiceMap, const char* serviceName) {
    struct StringToService* searchStringToService = (struct StringToService*)malloc(sizeof(struct StringToService));
    Service* foundService = NULL;
    InitStringToService(searchStringToService, serviceName, NULL);
    struct StringToService* foundStringToService = ((struct StringToService*)hashmap_get(stringToServiceMap, searchStringToService));
    if(foundStringToService != NULL) {
        foundService = foundStringToService->service;
    }
    FreeTempStringToService(searchStringToService);
    return foundService;
}

Trace* FindTrace(struct hashmap* stringToTraceMap, const char* traceId) {
    struct StringToTrace* stringToTrace = (struct StringToTrace*)malloc(sizeof(struct StringToTrace));
    InitStringToTrace(stringToTrace, traceId, NULL);
    Trace* foundTrace = ((const struct StringToTrace*)hashmap_get(stringToTraceMap, stringToTrace)) -> trace;
    FreeTempStringToTrace(stringToTrace);
    return foundTrace;
}