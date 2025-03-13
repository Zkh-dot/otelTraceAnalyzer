#include "servicemap.h"

int StringToServiceCompare(const void *a, const void *b, void *udata) {
    const struct StringToService *ua = a;
    const struct StringToService *ub = b;
    return strcmp(ua->string, ub->string);
};

uint64_t StringToServiceHash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct StringToService *StringToInt = item;
    return hashmap_sip(StringToInt->string, strlen(StringToInt->string), seed0, seed1);
}

void InitStringToService(struct StringToService* stringToService, char* string, Service* service) {
    stringToService->string = strdup(string);
    stringToService->service = service;
}

struct hashmap* GetStringToServiceMap() {
    return hashmap_new(sizeof(struct StringToService), 0, 0, 0, 
        StringToServiceHash, StringToServiceCompare, NULL, NULL);
}

Service* AddNewService(struct hashmap* stringToServiceMap, char* serviceName) {
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

void FreeStringToServiceMap(struct hashmap* stringToServiceMap) {
    size_t iter = 0;
    void *item;
    while (hashmap_iter(stringToServiceMap, &iter, &item))  {
        FreeStringToService((struct StringToService*)item);
    }
    hashmap_free(stringToServiceMap);
}

Service* FindService(struct hashmap* stringToServiceMap, char* serviceName) {
    struct StringToService stringToService;
    InitStringToService(&stringToService, serviceName, NULL);
    Service* foundService = ((const struct StringToService*)hashmap_get(stringToServiceMap, &stringToService)) -> service;
    return foundService;
}