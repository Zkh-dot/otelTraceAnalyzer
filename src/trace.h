#include "./hashstructs/hashset.h"
#include "span.h"
#include <string>

extern const char* errorMessages[5];

typedef struct {
    char* traceString;
    char* serviceName;
    char* traceId;
    hashset_t* spanIds;
} Trace;

char *LongStrTok(const char *input, const char *delimiter);

int CountSpans(char* trace);

char* ScanTrace(const char *field, const char *trace);

Span* FindAllSpans(Trace* trace);