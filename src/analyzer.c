#include "counters.h"
#include "span.h"

int main() {
    Span* span = (Span*)malloc(sizeof(Span));
    char* spanId = (char*)"spanId";
    char* serviceName = (char*)"serviceName";
    Span* parentSpan = NULL;
    span = InitSpan(span, spanId, serviceName, parentSpan);

    spanId = (char*)""; serviceName = (char*)"";

    FreeSpan(span);
    return 0;
}
