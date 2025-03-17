#pragma once

#include <Python.h>
#include <structmember.h>
#include "../structures/counters.h"
#include "../structures/span.h"
#include "../structures/trace.h"
#include "../structures/service.h"
#include "../structures/servicemap.h"

// counters
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

extern PyMethodDef PyCounters_methods[1];

extern PyMemberDef PyCounters_members[9];

extern PyTypeObject PyCountersType;

// trace
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

extern PyMethodDef PyTrace_methods[1];

extern PyMemberDef PyTrace_members[5];

extern PyTypeObject PyTraceType;

// service
typedef struct {
    PyObject_HEAD
    Service* _service;
    PyCounters* errorCounters;
    PyObject* serviceName;
} PyService;

void PyService_dealloc(PyService* self);

PyObject* PyService_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PyService_init(PyService* self, PyObject* args, PyObject* kwds);

void _updateService(PyService* self);

void setService4PyService(PyService* self, Service* service);

extern PyMethodDef PyService_methods[1];

extern PyMemberDef PyService_members[3];

extern PyTypeObject PyServiceType;// = {
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
