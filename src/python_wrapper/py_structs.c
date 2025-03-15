#include "py_structs.h"

void PyCounters_dealloc(PyCounters* self) {
    free(self->_statusCounter);
    Py_DECREF(self->myBadTraceExamples);
    Py_DECREF(self->notmyBadTraceExamples);
    Py_DECREF(self->myExamplesCount);
    Py_DECREF(self->notmyExamplesCount);
    Py_DECREF(self->serviceName);
    Py_DECREF(self->badTraceCount);
    Py_DECREF(self->mySpanCount);
    Py_DECREF(self->traceCount);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyCounters_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyCounters* self;
    self = (PyCounters*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->_statusCounter = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
        self->myBadTraceExamples = PyList_New(0);
        self->notmyBadTraceExamples = PyList_New(0);
        self->myExamplesCount = PyLong_FromLong(0);
        self->notmyExamplesCount = PyLong_FromLong(0);
        self->serviceName = PyUnicode_FromString("");
        self->badTraceCount = PyLong_FromLong(0);
        self->mySpanCount = PyLong_FromLong(0);
        self->traceCount = PyLong_FromLong(0);
    }
    return (PyObject*)self;
}

int PyCounters_init(PyCounters* self, PyObject* args, PyObject* kwds) {
    return 0;
}

void _updateCounter(PyCounters* self) {    
    self->_statusCounter->badTraceCount = (int)self->badTraceCount;
    self->_statusCounter->mySpanCount = (int)self->mySpanCount;
    self->_statusCounter->traceCount = (int)self->traceCount;
    self->_statusCounter->myExamplesCount = (int)self->myExamplesCount;
    self->_statusCounter->notmyExamplesCount = (int)self->notmyExamplesCount;
    self->_statusCounter->serviceName = (char*)PyUnicode_AsUTF8(self->serviceName);
    PyObject* iter = PyObject_GetIter(self->myBadTraceExamples);
    PyObject* item;
    while ((item = PyIter_Next(iter))) {
        self->_statusCounter->myBadTraceExamples[self->_statusCounter->myExamplesCount] = (char*)PyUnicode_AsUTF8(item);
        Py_DECREF(item);
        self->_statusCounter->myExamplesCount++;
    }
    iter = PyObject_GetIter(self->notmyBadTraceExamples);
    while ((item = PyIter_Next(iter))) {
        self->_statusCounter->notmyBadTraceExamples[self->_statusCounter->notmyExamplesCount] = (char*)PyUnicode_AsUTF8(item);
        Py_DECREF(item);
        self->_statusCounter->notmyExamplesCount++;
    }
    Py_DECREF(iter);
}

void _rupdateCounter(PyCounters* self) {
    self->badTraceCount = PyLong_FromLong(self->_statusCounter->badTraceCount);
    self->mySpanCount = PyLong_FromLong(self->_statusCounter->mySpanCount);
    self->traceCount = PyLong_FromLong(self->_statusCounter->traceCount);
    self->myExamplesCount = PyLong_FromLong(self->_statusCounter->myExamplesCount);
    self->notmyExamplesCount = PyLong_FromLong(self->_statusCounter->notmyExamplesCount);
    self->serviceName = PyUnicode_FromString(self->_statusCounter->serviceName);
    for (int i = 0; i < self->_statusCounter->myExamplesCount; i++) {
        PyList_Append(self->myBadTraceExamples, PyUnicode_FromString(self->_statusCounter->myBadTraceExamples[i]));
    }
    for (int i = 0; i < self->_statusCounter->notmyExamplesCount; i++) {
        PyList_Append(self->notmyBadTraceExamples, PyUnicode_FromString(self->_statusCounter->notmyBadTraceExamples[i]));
    }
}

// void PySpan_dealloc(PySpan* self) {
//     free(self->_span);
//     Py_DECREF(self->spanId);
//     Py_DECREF(self->serviceName);
//     Py_DECREF(self->parentSpanId);
//     Py_DECREF(self->traceId);
//     Py_DECREF(self->parentSpan);
//     Py_DECREF(self->spanStatus);
//     Py_TYPE(self)->tp_free((PyObject*)self);
// }

// PyObject* PySpan_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
//     PySpan* self;
//     self = (PySpan*)type->tp_alloc(type, 0);
//     if (self != NULL) {
//         self->_span = (Span*)malloc(sizeof(Span));
//         self->spanId = PyUnicode_FromString("");
//         self->serviceName = PyUnicode_FromString("");
//         self->parentSpanId = PyUnicode_FromString("");
//         self->traceId = PyUnicode_FromString("");
//         self->parentSpan = Py_None;
//         self->spanStatus = PyLong_FromLong(0);
//     }
//     return (PyObject*)self;
// }

// int PySpan_init(PySpan* self, PyObject* args, PyObject* kwds) {
//     return 0;
// }

// void _updateSpan(PySpan* self) {
//     self->_span->spanId = (char*)PyUnicode_AsUTF8(self->spanId);
//     self->_span->serviceName = (char*)PyUnicode_AsUTF8(self->serviceName);
//     self->_span->parentSpanId = (char*)PyUnicode_AsUTF8(self->parentSpanId);
//     self->_span->traceId = (char*)PyUnicode_AsUTF8(self->traceId);
//     self->_span->parentSpan = (Span*)PyLong_AsLong(self->parentSpan);
//     self->_span->spanStatus = (SpanStatusTypes)PyLong_AsLong(self->spanStatus);
// }

// void _rupdateSpan(PySpan* self) {
//     self->spanId = PyUnicode_FromString(self->_span->spanId);
//     self->serviceName = PyUnicode_FromString(self->_span->serviceName);
//     self->parentSpanId = PyUnicode_FromString(self->_span->parentSpanId);
//     self->traceId = PyUnicode_FromString(self->_span->traceId);
//     self->parentSpan = PyLong_FromLong((long)self->_span->parentSpan);
//     self->spanStatus = PyLong_FromLong((long)self->_span->spanStatus);
// }

void PyTrace_dealloc(PyTrace* self) {
    free(self->_trace);
    Py_DECREF(self->traceString);
    Py_DECREF(self->serviceName);
    Py_DECREF(self->traceId);
    Py_DECREF(self->spansCount);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyTrace_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyTrace* self;
    self = (PyTrace*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->_trace = (Trace*)malloc(sizeof(Trace));
        self->traceString = PyUnicode_FromString("");
        self->serviceName = PyUnicode_FromString("");
        self->traceId = PyUnicode_FromString("");
        self->spansCount = PyLong_FromLong(0);
    }
    return (PyObject*)self;
}

int PyTrace_init(PyTrace* self, PyObject* args, PyObject* kwds) {
    return 0;
}

void _updateTrace(PyTrace* self) {
    self->_trace->traceString = (char*)PyUnicode_AsUTF8(self->traceString);
    self->_trace->serviceName = (char*)PyUnicode_AsUTF8(self->serviceName);
    self->_trace->traceId = (char*)PyUnicode_AsUTF8(self->traceId);
    self->_trace->spansCount = (int)PyLong_AsLong(self->spansCount);
}

void _rupdateTrace(PyTrace* self) {
    self->traceString = PyUnicode_FromString(self->_trace->traceString);
    self->serviceName = PyUnicode_FromString(self->_trace->serviceName);
    self->traceId = PyUnicode_FromString(self->_trace->traceId);
    self->spansCount = PyLong_FromLong(self->_trace->spansCount);
}

// void PyService_dealloc(PyService* self) {
//     free(self->_service);
//     Py_DECREF(self->serviceName);
//     Py_DECREF(self->errorCounters);
//     Py_DECREF(self->traces);
//     Py_TYPE(self)->tp_free((PyObject*)self);
// }

// PyObject* PyService_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
//     PyService* self;
//     self = (PyService*)type->tp_alloc(type, 0);
//     if (self != NULL) {
//         self->_service = (Service*)malloc(sizeof(Service));
//         self->serviceName = PyUnicode_FromString("");
//         self->errorCounters = Py_None;
//         self->traces = PyList_New(0);
//     }
//     return (PyObject*)self;
// }

// int PyService_init(PyService* self, PyObject* args, PyObject* kwds) {
//     return 0;
// }

// void _updateService(PyService* self) {
//     self->_service->serviceName = (char*)PyUnicode_AsUTF8(self->serviceName);
//     self->_service->errorCounters = (ServiceErrorCounters*)PyLong_AsLong(self->errorCounters);
//     PyObject* iter = PyObject_GetIter(self->traces);
//     PyObject* item;
//     while ((item = PyIter_Next(iter))) {
//         self->_service->traces[(int)iter] = (Trace*)PyLong_AsLong(item);
//         Py_DECREF(item);
//     }
//     Py_DECREF(iter);
// }

// void _rupdateService(PyService* self) {
//     self->serviceName = PyUnicode_FromString(self->_service->serviceName);
//     self->errorCounters = PyLong_FromLong((long)self->_service->errorCounters);
//     Py_DECREF(self->traces);
//     self->traces = PyList_New(0);
//     for (int i = 0; i < sizeof(self->_service->traces) / sizeof(Trace*); i++) {
//         PyList_Append(self->traces, PyLong_FromLong((long)self->_service->traces[i]));
//     }
// }


const PyMethodDef PyCounters_methods[] = {
    {"_update", (PyCFunction)_updateCounter, METH_NOARGS, "Update counter"},
    {"_rupdate", (PyCFunction)_rupdateCounter, METH_NOARGS, "Reverse update counter"},
    {NULL}
};

const PyTypeObject PyCountersType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.Counters",
    .tp_doc = "Counters objects",
    .tp_basicsize = sizeof(PyCounters),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyCounters_new,
    .tp_init = (initproc)PyCounters_init,
    .tp_dealloc = (destructor)PyCounters_dealloc,
    .tp_methods = PyCounters_methods,
};

const PyMethodDef PyTrace_methods[] = {
    {"_update", (PyCFunction)_updateTrace, METH_NOARGS, "Update trace"},
    {"_rupdate", (PyCFunction)_rupdateTrace, METH_NOARGS, "Reverse update trace"},
    {NULL}
};

const PyTypeObject PyTraceType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.Trace",
    .tp_doc = "Trace objects",
    .tp_basicsize = sizeof(PyTrace),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyTrace_new,
    .tp_init = (initproc)PyTrace_init,
    .tp_dealloc = (destructor)PyTrace_dealloc,
    .tp_methods = PyTrace_methods,
};