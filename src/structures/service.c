#include "service.h"

void InitService(Service* service, const char* serviceName) {
    service->serviceName = strdup(serviceName);
    service->errorCounters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    InitServiceErrorCounters(service->errorCounters);
}

void FreeService(Service* service) {
    free(service->serviceName);
    FreeServiceErrorCounters(service->errorCounters);
    free(service);
}