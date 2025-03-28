#include "py_structs.h"
#include <structmember.h>

// counters
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
    Py_DECREF(self->statusCounter);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyCounters_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyCounters* self;
    self = (PyCounters*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->myBadTraceExamples = PyList_New(0);
        self->statusCounter = PyDict_New();
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
    self->badTraceCount =       Py_BuildValue("i", self->_statusCounter->badTraceCount);
    self->mySpanCount =         Py_BuildValue("i", self->_statusCounter->mySpanCount);
    self->traceCount =          Py_BuildValue("i", self->_statusCounter->traceCount);
    self->myExamplesCount =     Py_BuildValue("i", self->_statusCounter->myExamplesCount);
    self->notmyExamplesCount =  Py_BuildValue("i", self->_statusCounter->notmyExamplesCount);

    self->myBadTraceExamples = PyList_New(0);
    self->notmyBadTraceExamples = PyList_New(0);
    self->statusCounter = PyDict_New();

    for (int i = 0; i < self->_statusCounter->myExamplesCount; i++) {
        PyList_Append(
            self->myBadTraceExamples,
            Py_BuildValue(
                "s",
                self->_statusCounter->myBadTraceExamples[i],
                strlen(self->_statusCounter->myBadTraceExamples[i])
            )
        );
    }
    for (int i = 0; i < self->_statusCounter->notmyExamplesCount; i++) {
        PyList_Append(self->notmyBadTraceExamples, Py_BuildValue("s", self->_statusCounter->notmyBadTraceExamples[i], strlen(self->_statusCounter->notmyBadTraceExamples[i])));
    }
    
    for(int i = 0; i < TraceOk + 1; i++) {
        PyDict_SetItem(
            self->statusCounter,
            Py_BuildValue("s", traceStatusMessage[i], strlen(traceStatusMessage[i])),
            Py_BuildValue("i", self->_statusCounter->statusCounter[i])
        );
    }
}

void _rupdateCounter(PyCounters* self) {
    self->_statusCounter->badTraceCount =       (int)PyLong_AsLong(self->badTraceCount);
    self->_statusCounter->mySpanCount =         (int)PyLong_AsLong(self->mySpanCount);
    self->_statusCounter->traceCount =          (int)PyLong_AsLong(self->traceCount);
    self->_statusCounter->myExamplesCount =     (int)PyLong_AsLong(self->myExamplesCount);
    self->_statusCounter->notmyExamplesCount =  (int)PyLong_AsLong(self->notmyExamplesCount);

    for (int i = 0; i < self->_statusCounter->myExamplesCount; i++) {
        self->_statusCounter->myBadTraceExamples[i] = (char*)PyUnicode_AsUTF8(PyList_GetItem(self->myBadTraceExamples, i));
    }
    for (int i = 0; i < self->_statusCounter->notmyExamplesCount; i++) {
        self->_statusCounter->notmyBadTraceExamples[i] = (char*)PyUnicode_AsUTF8(PyList_GetItem(self->notmyBadTraceExamples, i));
    }
    for (int i = 0; i < TraceOk + 1; i ++) {
        self->_statusCounter->statusCounter[i] = (int)PyLong_AsLong(PyDict_GetItemString(self->statusCounter, traceStatusMessage[i]));
    }
}

void setCounters4PyCounters(PyCounters* self, ServiceErrorCounters* counters) {
    self->_statusCounter = counters;
    _updateCounter(self);
}

PyMethodDef PyCounters_methods[] = {
    {NULL}
};

PyMemberDef PyCounters_members[] = {
    {"myBadTraceExamples", T_OBJECT_EX, offsetof(PyCounters, myBadTraceExamples), 0, "My bad trace examples"},
    {"notmyBadTraceExamples", T_OBJECT_EX, offsetof(PyCounters, notmyBadTraceExamples), 0, "Not my bad trace examples"},
    {"myExamplesCount", T_OBJECT_EX, offsetof(PyCounters, myExamplesCount), 0, "My examples count"},
    {"notmyExamplesCount", T_OBJECT_EX, offsetof(PyCounters, notmyExamplesCount), 0, "Not my examples count"},
    {"serviceName", T_OBJECT_EX, offsetof(PyCounters, serviceName), 0, "Service name"},
    {"badTraceCount", T_OBJECT_EX, offsetof(PyCounters, badTraceCount), 0, "Bad trace count"},
    {"mySpanCount", T_OBJECT_EX, offsetof(PyCounters, mySpanCount), 0, "My span count"},
    {"traceCount", T_OBJECT_EX, offsetof(PyCounters, traceCount), 0, "Trace count"},
    {"statusCounters", T_OBJECT_EX, offsetof(PyCounters, statusCounter), 0, "Trace count"},
    {NULL}
};

PyTypeObject PyCountersType = {
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
    .tp_members = PyCounters_members,
};

PyMethodDef PyTrace_methods[] = {
    {NULL}
};


// trace
void PyTrace_dealloc(PyTrace* self) {
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
    self->_trace = (Trace*)malloc(sizeof(Trace));
    char* traceString = (char*)PyUnicode_AsUTF8(self->traceString);
    char* serviceName = (char*)PyUnicode_AsUTF8(self->serviceName);
    char* traceId = (char*)PyUnicode_AsUTF8(self->traceId);
    InitTrace(self->_trace, traceString, serviceName, traceId);
}

void _rupdateTrace(PyTrace* self) {
    self->serviceName = PyUnicode_FromString(self->_trace->serviceName);
    self->traceId = PyUnicode_FromString(self->_trace->traceId);
    self->spansCount = PyLong_FromLong(self->_trace->spansCount);
}

PyMemberDef PyTrace_members[] = {
    {"traceString", T_OBJECT_EX, offsetof(PyTrace, traceString), 0, "Trace string"},
    {"serviceName", T_OBJECT_EX, offsetof(PyTrace, serviceName), 0, "Service name"},
    {"traceId", T_OBJECT_EX, offsetof(PyTrace, traceId), 0, "Trace ID"},
    {"spansCount", T_OBJECT_EX, offsetof(PyTrace, spansCount), 0, "Spans count"},
    {"spansList", T_OBJECT_EX, offsetof(PyTrace, spansList), 0, "Spans list"},
    {NULL}
};

PyTypeObject PyTraceType = {
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
    .tp_members = PyTrace_members,
};


// service
void PyService_dealloc(PyService* self) {
    Py_DECREF(self->errorCounters);
    Py_DECREF(self->serviceName);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyService_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyService* self;
    self = (PyService*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->errorCounters = (PyCounters*)PyType_GenericAlloc(&PyCountersType, 0);
        self->serviceName = PyUnicode_FromString("");
    }
    return (PyObject*)self;
}

int PyService_init(PyService* self, PyObject* args, PyObject* kwds) {
    return 0;    
}

void _updateService(PyService* self) {
    self->_service = (Service*)malloc(sizeof(Service));
    char* serviceName = (char*)PyUnicode_AsUTF8(self->serviceName);
    InitService(self->_service, serviceName);
}

void setService4PyService(PyService* self, Service* service) {
    self->_service = service;
    self->serviceName = PyUnicode_FromString(service->serviceName);
    self->errorCounters = (PyCounters*)PyType_GenericAlloc(&PyCountersType, 0);
    setCounters4PyCounters(self->errorCounters, service->errorCounters);
}

PyMethodDef PyService_methods[] = {
    {NULL}
};

PyMemberDef PyService_members[] = {
    {"errorCounters", T_OBJECT_EX, offsetof(PyService, errorCounters), 0, "Error counters"},
    {"serviceName", T_OBJECT_EX, offsetof(PyService, serviceName), 0, "Service name"},
    {NULL}
};

PyTypeObject PyServiceType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "pywrapper.Service",
        .tp_doc = "Service objects",
        .tp_basicsize = sizeof(PyService),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT,
        .tp_new = PyService_new,
        .tp_init = (initproc)PyService_init,
        .tp_dealloc = (destructor)PyService_dealloc,
        .tp_methods = PyService_methods,
        .tp_members = PyService_members,
};