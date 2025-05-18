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

// char *LongStrTok(char *input, const char *delimiter) {
//     static char *string = NULL;
//     if (input != NULL) {
//         string = strdup(input);
//     }
//     if (string == NULL)
//         return string;

//     char *end = strstr(string, delimiter);
//     if (end == NULL) {
//         char *temp = string;
//         string = NULL;
//         return temp;
//     }

//     *end = '\0';
//     string = end + strlen(delimiter);
//     return string;
// }

char* LongStrTok(char *input, const char * delimeter) {
    char *found = strstr(input, delimeter);
    if (found == NULL) {
        return NULL;
    }
    *found = '\0';
    return found + strlen(delimeter);
}

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

void FindAllSpans(Trace* trace) {
    trace->spansCount = CountSpans(trace->traceString);
    trace->spans = (Span**)malloc(trace->spansCount * sizeof(Span*));
    
    const char* Delimiter = "}, {";
    char* tempCopy = trace->traceString;
    char* token = LongStrTok(tempCopy, Delimiter);
    int i;
    for(i = 0; i < trace->spansCount; i++) {
        char* spanId = ScanTrace(SPAN_ID_KEY, tempCopy);
        char* serviceName = ScanTrace(SERVICE_NAME_KEY, tempCopy);
        char* spanParentId = ScanTrace(PARENT_SPAN_ID_KEY, tempCopy);
        if(spanId != NULL && serviceName != NULL) {
            Span* span = (Span*)malloc(sizeof(Span));
            InitSpan(span, spanId, serviceName, spanParentId, NULL);
            trace->spans[i] = span;
            // if(hashset_is_member(trace->spanIds, trace->spans[i]->spanId) == 0)
            hashset_add(trace->spanIds, hash16digits(trace->spans[i]->spanId));
            printf("add spanId: %s, %d\n", trace->spans[i]->spanId, hashset_is_member(trace->spanIds, hash16digits(trace->spans[i]->spanId)));
            free(spanId);
            free(serviceName);
        }
        if(spanParentId != NULL){
            free(spanParentId);
        }    
        if (token == NULL) {
            break;
        }
        tempCopy = token;
        token = LongStrTok(token, Delimiter);
    }
    trace->spansCount = i + 1;

    free(trace->traceString);
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
