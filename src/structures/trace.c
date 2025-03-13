#include "trace.h"

const char* traceStatusMessage[TraceOk + 1] = {
    "UndefTraceStatus",

    "myMissingParent",
    "notmyMissingParent",

    "myNoParentInTrace",
    "notmyNoParentInTrace",

    "myDublicateSpan",
    "notmyDublicateSpan",

    "myBadSpanIdSize",
    "notmyBadSpanIdSize",

    "myBadTraceIdSize",
    "notmyBadTraceIdSize",

    "TraceOk",
};

char *LongStrTok(char *input, const char *delimiter) {
    static char *string;
    if (input != NULL) {
        string = strdup(input);
        free(input);
    }
    if (string == NULL)
        return string;

    char *end = strstr(string, delimiter);
    if (end == NULL) {
        char *temp = string;
        string = NULL;
        return temp;
    }

    char *temp = string;

    *end = '\0';
    string = end + strlen(delimiter);
    return temp;
}

int CountSpans(char* trace) {
    int count = 0;
    const char *spanId_key = "'spanId': '";
    char *trace_copy = strdup(trace);
    if (trace_copy == NULL) {
        return -1;
    }

    char *pos = trace_copy;
    while ((pos = strstr(pos, spanId_key)) != NULL) {
        count++;
        pos += strlen(spanId_key);
    }

    free(trace_copy);
    return count;
}

char* ScanTrace(const char *field, const char *trace) {
    char *TraceCopy = strdup(trace);
    if (TraceCopy == NULL) {
        return NULL;
    }

    const char *key_prefix = "': '";
    const char *key_suffix = "'";
    const char *value_suffix = "'";

    char search_key[256];
    snprintf(search_key, sizeof(search_key), "'%s%s", field, key_prefix);

    char *key_pos = strstr(TraceCopy, search_key);
    if (key_pos == NULL) {
        free(TraceCopy);
        return NULL;
    }

    key_pos += strlen(search_key);

    char *value_end = strstr(key_pos, value_suffix);
    if (value_end == NULL) {
        free(TraceCopy);
        return NULL;
    }

    size_t value_length = value_end - key_pos;

    char *value = (char *)malloc((value_length + 1) * sizeof(char));
    if (value == NULL) {
        free(TraceCopy);
        return NULL;
    }

    strncpy(value, key_pos, value_length);
    value[value_length] = '\0';

    free(TraceCopy);
    return value;
}

Span** FindAllSpans(Trace* trace) {
    int SpansCount = CountSpans(trace->traceString);
    Span** spans = (Span**)malloc(SpansCount * sizeof(Span*));
    
    const char* Delimiter = "}, {";
    char* token = LongStrTok(trace->traceString, Delimiter);
    int i = 0;
    char *tmp;
    while (token != NULL && i < SpansCount) {
        char* spanId = ScanTrace("spanId", token);
        char* serviceName = ScanTrace("serviceName", token);
        InitSpan(spans[i], spanId, serviceName, NULL);
        hashset_add(trace->spanIds, spans[i]->spanId);
        i++;
        token = LongStrTok(NULL, Delimiter);
    }
    free(token);
    return spans;
}

void InitTrace(Trace* trace, char* traceString, char* serviceName, char* traceId) {
    trace->traceString = strdup(traceString);
    trace->serviceName = strdup(serviceName);
    trace->traceId = strdup(traceId);
    trace->spanIds = hashset_create();
}

void FreeTrace(Trace* trace) {
    free(trace->traceString);
    free(trace->serviceName);
    free(trace->traceId);
    hashset_destroy(trace->spanIds);
    free(trace);
}