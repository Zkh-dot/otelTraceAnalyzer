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
