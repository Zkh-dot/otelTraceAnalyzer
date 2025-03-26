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

int CountSpans(const char* trace) {
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

void SplitTrace(char *input, const char *delimiter, int spanCount, char** result) {
    char *current = input;
    result[0] = current;

    for (int i = 0; i < spanCount; i++) {
        char *delim_pos = strstr(current, delimiter);
        if (delim_pos == NULL) {
            break;
        }
        *delim_pos = '\0';

        current = delim_pos + strlen(delimiter);
        result[i + 1] = current;
    }
}

void FindAllSpans(Trace* trace) {
    trace->spansCount = CountSpans(trace->traceString);
    trace->spans = (Span**)malloc(trace->spansCount * sizeof(Span*));
    printf("trace: %s\n", trace->traceString);
    
    const char* Delimiter = "}, {";
    // char* tempCopy = strdup(trace->traceString);
    char** chunkPtrs = malloc((trace->spansCount) * sizeof(char*));
    SplitTrace(trace->traceString, Delimiter, trace->spansCount, chunkPtrs);
    for(int i = 0; i < trace->spansCount; i++) {
        char* spanId = ScanTrace(SPAN_ID_KEY, chunkPtrs[i]);
        char* serviceName = ScanTrace(SERVICE_NAME_KEY, chunkPtrs[i]);
        char* spanParentId = ScanTrace(PARENT_SPAN_ID_KEY, chunkPtrs[i]);
        Span* span = (Span*)malloc(sizeof(Span));
        InitSpan(span, spanId, serviceName, spanParentId, NULL);
        trace->spans[i] = span;
        if(hashset_is_member(trace->spanIds, trace->spans[i]->spanId) == 0)
            hashset_add(trace->spanIds, trace->spans[i]->spanId);
        
        free(spanId);
        free(serviceName);
        free(spanParentId);
    }
    free(trace->traceString);
    free(chunkPtrs);
    trace->traceString = NULL;
}

void InitTrace(
        Trace* trace,
        const char* traceString,
        const char* serviceName,
        const char* traceId
    ) {
    trace->traceString = strdup(traceString);
    trace->serviceName = strdup(serviceName);
    trace->traceId = strdup(traceId);
    trace->spanIds = hashset_create();
}

void FreeTrace(Trace* trace) {
    if(trace->traceString != NULL)
        free(trace->traceString);
    if(trace->spans != NULL)
        FreeAllSpans(trace->spans, trace->spansCount);
    free(trace->serviceName);
    free(trace->traceId);
    hashset_destroy(trace->spanIds);
    free(trace);
}
