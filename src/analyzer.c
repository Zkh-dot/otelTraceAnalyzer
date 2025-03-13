#include "structures/counters.h"
#include "structures/span.h"
#include "structures/trace.h"
#include "structures/service.h"
#include "structures/servicemap.h"

int main() {
    struct hashmap* stringToServiceMap = GetStringToServiceMap();
    char* serviceName = (char*)"serviceName";
    Service* service = AddNewService(stringToServiceMap, serviceName);
    
    Service* foundService = FindService(stringToServiceMap, serviceName);

    FreeStringToServiceMap(stringToServiceMap);
    return 0;
}
