#pragma once

#include <Python.h>
#include "../structures/counters.h"
#include "../structures/span.h"
#include "../structures/trace.h"
#include "../structures/service.h"
#include "../structures/servicemap.h"

typedef struct {
    PyObject_HEAD
    ServiceErrorCounters* statusCounter;
    PyObject* myBadTraceExamples;
    PyObject* notmyBadTraceExamples;
    PyObject* myExamplesCount;
    PyObject* notmyExamplesCount;
    PyObject* serviceName;
    PyObject* badTraceCount;
    PyObject* mySpanCount;
    PyObject* traceCount;
} PyCounters;

static void PyCounters_dealloc(PyCounters* self) {
    free(self->statusCounter);
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

static PyObject* PyCounters_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyCounters* self;
    self = (PyCounters*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->statusCounter = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
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

static int PyCounters_init(PyCounters* self, PyObject* args, PyObject* kwds) {
    return 0;
}

static void update(PyCounters* self) {
    ServiceErrorCounters* counters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    InitServiceErrorCounters(counters);
    
    counters->badTraceCount = (int)self->badTraceCount;
    counters->mySpanCount = (int)self->mySpanCount;
    counters->traceCount = (int)self->traceCount;
    counters->myExamplesCount = (int)self->myExamplesCount;
    counters->notmyExamplesCount = (int)self->notmyExamplesCount;
    counters->serviceName = PyUnicode_AsUTF8(self->serviceName);

}