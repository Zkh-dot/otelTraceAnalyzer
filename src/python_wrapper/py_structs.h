#pragma once

#include <Python.h>
#include <structmember.h>
#include <stdbool.h>
#include "../structures/counters.h"
#include "../structures/span.h"
#include "../structures/trace.h"
#include "../structures/service.h"
#include "../structures/servicemap.h"

// counters
typedef struct {
    PyObject_HEAD
    ServiceErrorCounters* _statusCounter;
    bool ownsStatusCounter;
    PyObject* myTraces;
    PyObject* notmyTraces;
    PyObject* serviceName;
    PyObject* badTraceCount;
    PyObject* traceCount;
    PyObject* inTraceSpanCount;
} PyServiceErrorCounters;

void PyServiceErrorCounters_dealloc(PyServiceErrorCounters* self);

PyObject* PyServiceErrorCounters_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PyServiceErrorCounters_init(PyServiceErrorCounters* self, PyObject* args, PyObject* kwds);

void _updatePyCounter(PyServiceErrorCounters* self, ServiceErrorCounters* counters);

void _updateCCounter(PyServiceErrorCounters* self);

extern PyMethodDef PyServiceErrorCounters_methods[1];

extern PyMemberDef PyServiceErrorCounters_members[7];

extern PyTypeObject PyServiceErrorCountersType;

// span

typedef struct {
    PyObject_HEAD
    Span* _span;
    PyObject* spanId;
    PyObject* serviceName;
    PyObject* parentSpanId;
    PyObject* traceId;
} PySpan;

void PySpan_dealloc(PySpan* self);

PyObject* PySpan_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PySpan_init(PySpan* self, PyObject* args, PyObject* kwds);

void _updatePySpan(PySpan* self, Span* span);

extern PyMethodDef PySpan_methods[1];

extern PyMemberDef PySpan_members[5];

extern PyTypeObject PySpanType;

// trace
typedef struct {
    PyObject_HEAD
    Trace* _trace;
    PyObject* traceString;
    PyObject* serviceName;
    PyObject* traceId;
    PyObject* spansCount;
    PyObject* spansList;
} PyTrace;

void PyTrace_dealloc(PyTrace* self);

PyObject* PyTrace_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PyTrace_init(PyTrace* self, PyObject* args, PyObject* kwds);

void _updateCTrace(PyTrace* self);

void _updatePyTrace(PyTrace* self, Trace* trace);

extern PyMethodDef PyTrace_methods[1];

extern PyMemberDef PyTrace_members[6];

extern PyTypeObject PyTraceType;

// service
typedef struct {
    PyObject_HEAD
    Service* _service;
    PyServiceErrorCounters* errorCounters;
    PyObject* serviceName;
} PyService;

void PyService_dealloc(PyService* self);

PyObject* PyService_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PyService_init(PyService* self, PyObject* args, PyObject* kwds);

void _updateCService(PyService* self);

void _updatePyService(PyService* self, Service* service);

PyObject* PyService_is_ok(PyService* self, PyObject* ignored);

extern PyMethodDef PyService_methods[2];

extern PyMemberDef PyService_members[3];

extern PyTypeObject PyServiceType;
