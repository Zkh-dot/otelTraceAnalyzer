#include "py_translator.h"

ServiceErrorCounters* PyCounters2Counters(PyObject* src) {
    
    return ((PyCounters*)src)->_statusCounter;
}

PyObject* Counters2PyCounters(PyCounters* dst, ServiceErrorCounters* src) {
    // PyCounters* pyCounters = (PyCounters*)PyType_GenericAlloc(&PyCountersType, 0);
    // Py_INCREF(pyCounters);
    setCounters4PyCounters(dst, src);
    return (PyObject*)dst;
}


PyObject* Counters2Dict(ServiceErrorCounters* counters) {
    PyObject* dict = PyDict_New();
    printf("%d\n", PyDict_Check(dict)); // to delete
    for(int i = 0; i < TraceOk + 1; i++) {
        PyDict_SetItemString(dict, traceStatusMessage[i], PyLong_FromLong(counters->statusCounter[i]));
    }
    // printf("%d\n", PyDict_Check(dict)); // to delete


    // TODO: fix
    // PyObject* myExamples = PyList_New(counters->myExamplesCount);
    // for(int i = 0; i < counters->myExamplesCount; i++) {
    //     PyList_SetItem(myExamples, i, PyUnicode_FromString(counters->myBadTraceExamples[i]));
    // }
    // printf("%d\n", PyDict_Check(dict)); // to delete
    // PyObject* notmyExamples = PyList_New(counters->notmyExamplesCount);
    // for(int i = 0; i < counters->notmyExamplesCount; i++) {
    //     PyList_SetItem(notmyExamples, i, PyUnicode_FromString(counters->notmyBadTraceExamples[i]));
    // }
    // printf("%d\n", PyDict_Check(dict)); // to delete
    // PyDict_SetItemString(dict, "myExamples", myExamples);
    // PyDict_SetItemString(dict, "notmyExamples", notmyExamples);
    PyDict_SetItemString(dict, "badTraceCount", PyLong_FromLong(counters->badTraceCount));
    PyDict_SetItemString(dict, "mySpanCount", PyLong_FromLong(counters->mySpanCount));
    PyDict_SetItemString(dict, "traceCount", PyLong_FromLong(counters->traceCount));
    printf("%d\n", PyDict_Check(dict)); // to delete
    
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

PyObject* Trace2PyTrace(PyTrace* dst, Trace* src) {
    // PyTrace* pyTrace = (PyTrace*)PyTrace_new(&PyTraceType, NULL, NULL);
    // Py_INCREF(pyTrace);
    dst->_trace = src;
    return (PyObject*)dst;
}

// Service* PyService2Service(PyObject* service) {
//     return ((PyService*)service)->_service;
// }

// PyObject* Service2PyService(Service* service) {
//     PyService* pyService = (PyService*)PyService_new(&PyServiceType, NULL, NULL);
//     pyService->_service = service;
//     return (PyObject*)pyService;
// }
