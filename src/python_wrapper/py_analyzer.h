#pragma once

#include <Python.h>
#include "../analyzer.h"
#include "py_structs.h"
#include "py_translator.h"

typedef struct {
    PyObject_HEAD
    Analyzer* analyzer;
} PyAnalyzer;

void PyAnalyzer_dealloc(PyAnalyzer* self);

PyObject* PyAnalyzer_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PyAnalyzer_init(PyAnalyzer* self, PyObject* args, PyObject* kwds);

PyObject* PyGetServiceErrorCounters(PyAnalyzer* self, ServiceErrorCounters* counters);

PyObject* PyAPIAnalyzeTrace(PyAnalyzer* self, PyObject* args);

PyObject* PyAPIAnalyzeTraceBTrace(PyAnalyzer* self, PyTrace* trace);

PyObject* PyAPIGetServiceErrorCounters(PyAnalyzer* self, PyObject* args);

PyObject* PyAPIGetServiceErrorCountersObj(PyAnalyzer* self, PyObject* args);

PyObject* PyAPIGetAllServiceErrorCounters(PyAnalyzer* self);

PyObject* PyAPIGetAllServiceErrorCountersObj(PyAnalyzer* self);

extern PyMethodDef PyAnalyzer_methods[7];

extern PyTypeObject PyAnalyzerType;

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
