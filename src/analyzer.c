#include "structures/counters.h"
#include "structures/span.h"
#include "structures/trace.h"
#include "structures/service.h"
#include "structures/servicemap.h"

int main() {
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    char* traceString = (char*)"[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]";
    char* serviceName = (char*)"serviceName";
    char* traceId = (char*)"traceId";
    InitTrace(trace, traceString, serviceName, traceId);
    Span** spans = FindAllSpans(trace);
    FreeAllSpans(spans, trace->spansCount);
    FreeTrace(trace);
    return 0;
}
