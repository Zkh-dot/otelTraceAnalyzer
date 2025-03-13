#include "service.h"

void InitService(Service* service, char* serviceName) {
    service->serviceName = (char*)malloc(strlen(serviceName) + 1);
    strcpy(service->serviceName, serviceName);
    service->errorCounters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    InitServiceErrorCounters(service->errorCounters);
}

void FreeService(Service* service) {
    free(service->serviceName);
    FreeServiceErrorCounters(service->errorCounters);
    free(service);
}