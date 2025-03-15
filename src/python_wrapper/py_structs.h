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

static void PyCounters_dealloc(PyCounters* self);

static PyObject* PyCounters_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

static int PyCounters_init(PyCounters* self, PyObject* args, PyObject* kwds);

static void _updateCounter(PyCounters* self);

static void _rupdateCounter(PyCounters* self);

typedef struct {
    PyObject_HEAD
    Span* _span;
    PyObject* spanId;
    PyObject* serviceName;
    PyObject* parentSpanId;
    PyObject* traceId;
    PyObject* parentSpan;
    PyObject* spanStatus;
} PySpan;

static void PySpan_dealloc(PySpan* self);

static PyObject* PySpan_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

static int PySpan_init(PySpan* self, PyObject* args, PyObject* kwds);

static void _updateSpan(PySpan* self);

static void _rupdateSpan(PySpan* self);

typedef struct {
    PyObject_HEAD
    Trace* _trace;
    PyObject* traceString;
    PyObject* serviceName;
    PyObject* traceId;
    PyObject* spansCount;
} PyTrace;

static void PyTrace_dealloc(PyTrace* self);

static PyObject* PyTrace_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

static int PyTrace_init(PyTrace* self, PyObject* args, PyObject* kwds);

static void _updateTrace(PyTrace* self);

static void _rupdateTrace(PyTrace* self);

typedef struct {
    PyObject_HEAD
    Service* _service;
    PyObject* serviceName;
    PyCounters* errorCounters;
    PyObject* traces;
} PyService;

static void PyService_dealloc(PyService* self);

static PyObject* PyService_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

static int PyService_init(PyService* self, PyObject* args, PyObject* kwds);

static void _updateService(PyService* self);

static void _rupdateService(PyService* self);

// static PyMethodDef PyCounters_methods[] = {
//     {"_update", (PyCFunction)_updateCounter, METH_NOARGS, "Update counter"},
//     {"_rupdate", (PyCFunction)_rupdateCounter, METH_NOARGS, "Reverse update counter"},
//     {NULL}
// };

// static PyTypeObject PyCountersType = {
//     PyVarObject_HEAD_INIT(NULL, 0)
//     .tp_name = "pywrapper.Counters",
//     .tp_doc = "Counters objects",
//     .tp_basicsize = sizeof(PyCounters),
//     .tp_itemsize = 0,
//     .tp_flags = Py_TPFLAGS_DEFAULT,
//     .tp_new = PyCounters_new,
//     .tp_init = (initproc)PyCounters_init,
//     .tp_dealloc = (destructor)PyCounters_dealloc,
//     .tp_methods = PyCounters_methods,
// };


// static PyMethodDef PySpan_methods[] = {
//     {"_update", (PyCFunction)_updateSpan, METH_NOARGS, "Update span"},
//     {"_rupdate", (PyCFunction)_rupdateSpan, METH_NOARGS, "Reverse update span"},
//     {NULL}
// };

// static PyTypeObject PySpanType = {
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


static PyMethodDef PyTrace_methods[] = {
    {"_update", (PyCFunction)_updateTrace, METH_NOARGS, "Update trace"},
    {"_rupdate", (PyCFunction)_rupdateTrace, METH_NOARGS, "Reverse update trace"},
    {NULL}
};

static PyTypeObject PyTraceType = {
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


// static PyMethodDef PyService_methods[] = {
//     {"_update", (PyCFunction)_updateService, METH_NOARGS, "Update service"},
//     {"_rupdate", (PyCFunction)_rupdateService, METH_NOARGS, "Reverse update service"},
//     {NULL}
// };

// static PyTypeObject PyServiceType = {
//     PyVarObject_HEAD_INIT(NULL, 0)
//     .tp_name = "pywrapper.Service",
//     .tp_doc = "Service objects",
//     .tp_basicsize = sizeof(PyService),
//     .tp_itemsize = 0,
//     .tp_flags = Py_TPFLAGS_DEFAULT,
//     .tp_new = PyService_new,
//     .tp_init = (initproc)PyService_init,
//     .tp_dealloc = (destructor)PyService_dealloc,
//     .tp_methods = PyService_methods,
// };
