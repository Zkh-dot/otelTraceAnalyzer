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
    PyObject* statusCounter;
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

void _rupdateCounter(PyCounters* self);

void setCounters4PyCounters(PyCounters* self, ServiceErrorCounters* counters);

extern PyMethodDef PyCounters_methods[1];

extern PyMemberDef PyCounters_members[10];

extern PyTypeObject PyCountersType;

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

void _updateSpan(PySpan* self);

extern PyMethodDef PySpan_methods[1];

extern PyMemberDef PySpan_members[4];

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

void _updateTrace(PyTrace* self);

void _rupdateTrace(PyTrace* self);

extern PyMethodDef PyTrace_methods[1];

extern PyMemberDef PyTrace_members[6];

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

extern PyTypeObject PyServiceType;
