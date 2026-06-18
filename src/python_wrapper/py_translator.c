#include "py_translator.h"

ServiceErrorCounters* PyServiceErrorCounters2Counters(PyObject* src) {
    
    return ((PyServiceErrorCounters*)src)->_statusCounter;
}

static int set_dict_item_steal(PyObject* dict, const char* key, PyObject* value) {
    if (value == NULL) {
        return -1;
    }
    int result = PyDict_SetItemString(dict, key, value);
    Py_DECREF(value);
    return result;
}

static PyObject* TraceStatusCounters2Dict(int* statusCounters) {
    PyObject* dict = PyDict_New();
    if (dict == NULL) {
        return NULL;
    }

    for(int i = 0; i < TraceOk + 1; i++) {
        if (set_dict_item_steal(dict, traceStatusMessage[i], PyLong_FromLong(statusCounters[i])) < 0) {
            Py_DECREF(dict);
            return NULL;
        }
    }

    return dict;
}

static PyObject* BadTraceExamples2List(char** examples, int examplesCount) {
    PyObject* list = PyList_New(examplesCount);
    if (list == NULL) {
        return NULL;
    }

    for(int i = 0; i < examplesCount; i++) {
        PyObject* value = PyUnicode_FromString(examples[i] != NULL ? examples[i] : "");
        if (value == NULL) {
            Py_DECREF(list);
            return NULL;
        }
        PyList_SET_ITEM(list, i, value);
    }

    return list;
}

static PyObject* ServiceCounters2Dict(ServiceCounters* counters, int* statusCounters) {
    PyObject* dict = PyDict_New();
    if (dict == NULL) {
        return NULL;
    }

    if (
        set_dict_item_steal(dict, "statusCounters", TraceStatusCounters2Dict(statusCounters)) < 0 ||
        set_dict_item_steal(dict, "myExamples", BadTraceExamples2List(counters->myBadTraceExamples, counters->myExamplesCount)) < 0 ||
        set_dict_item_steal(dict, "notmyExamples", BadTraceExamples2List(counters->notmyBadTraceExamples, counters->notmyExamplesCount)) < 0 ||
        set_dict_item_steal(dict, "myExamplesCount", PyLong_FromLong(counters->myExamplesCount)) < 0 ||
        set_dict_item_steal(dict, "notmyExamplesCount", PyLong_FromLong(counters->notmyExamplesCount)) < 0 ||
        set_dict_item_steal(dict, "mySpanCount", PyLong_FromLong(counters->mySpanCount)) < 0 ||
        set_dict_item_steal(dict, "notmySpanCount", PyLong_FromLong(counters->notmySpanCount)) < 0
    ) {
        Py_DECREF(dict);
        return NULL;
    }

    return dict;
}

PyObject* Counters2Dict(ServiceErrorCounters* counters) {
    PyObject* dict = PyDict_New();
    if (dict == NULL) {
        return NULL;
    }

    if (
        set_dict_item_steal(dict, "my-traces", ServiceCounters2Dict(&counters->myTraceServiceCounters, counters->myTraceStatusCounter)) < 0 ||
        set_dict_item_steal(dict, "notmy-traces", ServiceCounters2Dict(&counters->notmyTraceServiceCounters, counters->notmyTraceStatusCounter)) < 0 ||
        set_dict_item_steal(dict, "badTraceCount", PyLong_FromLong(counters->badTraceCount)) < 0 ||
        set_dict_item_steal(dict, "traceCount", PyLong_FromLong(counters->traceCount)) < 0 ||
        set_dict_item_steal(dict, "inTraceSpanCount", PyLong_FromLong(counters->inTraceSpanCount)) < 0
    ) {
        Py_DECREF(dict);
        return NULL;
    }

    if (counters->serviceName != NULL) {
        if (set_dict_item_steal(dict, "serviceName", PyUnicode_FromString(counters->serviceName)) < 0) {
            Py_DECREF(dict);
            return NULL;
        }
    }

    return dict;
}

// Span* PySpan2Span(PyObject* span) {
//     return ((PySpan*)span)->_span;
// }

// PyObject* Span2PySpan(Span* span) {
//     PySpan* pySpan = (PySpan*)PySpan_new(&PySpanType, NULL, NULL);
//     pySpan->_span = span;
//     return (PyObject*)pySpan;
// }

Trace* PyTrace2Trace(PyObject* src) {
    return ((PyTrace*)src)->_trace;
}

// Service* PyService2Service(PyObject* service) {
//     return ((PyService*)service)->_service;
// }

// PyObject* Service2PyService(Service* service) {
//     PyService* pyService = (PyService*)PyService_new(&PyServiceType, NULL, NULL);
//     pyService->_service = service;
//     return (PyObject*)pyService;
// }
