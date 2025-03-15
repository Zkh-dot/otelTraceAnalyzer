#include "py_translator.h"

ServiceErrorCounters* PyCounters2Counters(PyObject* counters) {
    return ((PyCounters*)counters)->_statusCounter;
}

PyObject* Counters2PyCounters(ServiceErrorCounters* counters) {
    PyCounters* pyCounters = (PyCounters*)PyCounters_new(&PyCountersType, NULL, NULL);
    Py_INCREF(pyCounters);
    pyCounters->_statusCounter = counters;
    return (PyObject*)pyCounters;
}

// Span* PySpan2Span(PyObject* span) {
//     return ((PySpan*)span)->_span;
// }

// PyObject* Span2PySpan(Span* span) {
//     PySpan* pySpan = (PySpan*)PySpan_new(&PySpanType, NULL, NULL);
//     pySpan->_span = span;
//     return (PyObject*)pySpan;
// }

Trace* PyTrace2Trace(PyObject* trace) {
    return ((PyTrace*)trace)->_trace;
}

PyObject* Trace2PyTrace(Trace* trace) {
    PyTrace* pyTrace = (PyTrace*)PyTrace_new(&PyTraceType, NULL, NULL);
    Py_INCREF(pyTrace);
    pyTrace->_trace = trace;
    return (PyObject*)pyTrace;
}

// Service* PyService2Service(PyObject* service) {
//     return ((PyService*)service)->_service;
// }

// PyObject* Service2PyService(Service* service) {
//     PyService* pyService = (PyService*)PyService_new(&PyServiceType, NULL, NULL);
//     pyService->_service = service;
//     return (PyObject*)pyService;
// }
