#pragma once

#include <Python.h>
#include "../structures/counters.h"
#include "../structures/span.h"
#include "../structures/trace.h"
#include "../structures/service.h"
#include "../structures/servicemap.h"

typedef struct {
    PyObject_HEAD
    ServiceErrorCounters* _statusCounter;
    PyObject* myBadTraceExamples;
    PyObject* notmyBadTraceExamples;
    PyObject* myExamplesCount;
    PyObject* notmyExamplesCount;
    PyObject* serviceName;
    PyObject* badTraceCount;
    PyObject* mySpanCount;
    PyObject* traceCount;
} PyCounters;

void PyCounters_dealloc(PyCounters* self);

PyObject* PyCounters_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PyCounters_init(PyCounters* self, PyObject* args, PyObject* kwds);

void _updateCounter(PyCounters* self);

void setCounters4PyCounters(PyCounters* self, ServiceErrorCounters* counters);

// typedef struct {
//     PyObject_HEAD
//     Span* _span;
//     PyObject* spanId;
//     PyObject* serviceName;
//     PyObject* parentSpanId;
//     PyObject* traceId;
//     PyObject* parentSpan;
//     PyObject* spanStatus;
// } PySpan;

// void PySpan_dealloc(PySpan* self);

// PyObject* PySpan_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

// int PySpan_init(PySpan* self, PyObject* args, PyObject* kwds);

// void _updateSpan(PySpan* self);

// void _rupdateSpan(PySpan* self);

typedef struct {
    PyObject_HEAD
    Trace* _trace;
    PyObject* traceString;
    PyObject* serviceName;
    PyObject* traceId;
    PyObject* spansCount;
} PyTrace;

void PyTrace_dealloc(PyTrace* self);

PyObject* PyTrace_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PyTrace_init(PyTrace* self, PyObject* args, PyObject* kwds);

void _updateTrace(PyTrace* self);

void _rupdateTrace(PyTrace* self);

// typedef struct {
//     PyObject_HEAD
//     Service* _service;
//     PyObject* serviceName;
//     PyCounters* errorCounters;
//     PyObject* traces;
// } PyService;

// void PyService_dealloc(PyService* self);

// PyObject* PyService_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

// int PyService_init(PyService* self, PyObject* args, PyObject* kwds);

// void _updateService(PyService* self);

// void _rupdateService(PyService* self);

extern PyMethodDef PyCounters_methods[3];

extern PyMemberDef PyCounters_members[9];

extern PyTypeObject PyCountersType;

// extern PyMethodDef PySpan_methods[];

// PyTypeObject PySpanType = {
//     PyVarObject_HEAD_INIT(NULL, 0)
//     .tp_name = "pywrapper.Span",
//     .tp_doc = "Span objects",
//     .tp_basicsize = sizeof(PySpan),
//     .tp_itemsize = 0,
//     .tp_flags = Py_TPFLAGS_DEFAULT,
//     .tp_new = PySpan_new,
//     .tp_init = (initproc)PySpan_init,
//     .tp_dealloc = (destructor)PySpan_dealloc,
//     .tp_methods = PySpan_methods,
// };


extern PyMethodDef PyTrace_methods[3];

extern PyMemberDef PyTrace_members[5];

extern PyTypeObject PyTraceType;


// PyMethodDef PyService_methods[] = {
//     {"_update", (PyCFunction)_updateService, METH_NOARGS, "Update service"},
//     {"_rupdate", (PyCFunction)_rupdateService, METH_NOARGS, "Reverse update service"},
//     {NULL}
// };

// PyTypeObject PyServiceType = {
//     PyVarObject_HEAD_INIT(NULL, 0)
//     .tp_name = "pywrapper.Service",
    // .tp_doc = "Service objects",
//     .tp_basicsize = sizeof(PyService),
//     .tp_itemsize = 0,
//     .tp_flags = Py_TPFLAGS_DEFAULT,
//     .tp_new = PyService_new,
//     .tp_init = (initproc)PyService_init,
//     .tp_dealloc = (destructor)PyService_dealloc,
//     .tp_methods = PyService_methods,
// };
