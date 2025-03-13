#include "trace.h"
#include "counters.h"

typedef struct {
    char* serviceName;
    ServiceErrorCounters* errorCounters;    
} Service;

Service* InitService(Service* service, char* serviceName);

void FreeService(Service* service);