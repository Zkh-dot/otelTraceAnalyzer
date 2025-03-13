#include "service.h"

Service* InitService(Service* service, char* serviceName) {
    service->serviceName = (char*)malloc(strlen(serviceName) + 1);
    strcpy(service->serviceName, serviceName);
    service->errorCounters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    service->errorCounters = InitServiceErrorCounters(service->errorCounters);
    return service;
}

void FreeService(Service* service) {
    free(service->serviceName);
    FreeServiceErrorCounters(service->errorCounters);
    free(service);
}