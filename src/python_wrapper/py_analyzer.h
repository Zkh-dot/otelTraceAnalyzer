#pragma once

#include <Python.h>
#include "../analyzer.h"
#include "py_structs.h"

typedef struct {
    PyObject_HEAD
    Analyzer* analyzer;
} PyAnalyzer;

static void PyAnalyzer_dealloc(PyAnalyzer* self);

static PyObject* PyAnalyzer_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

static int PyAnalyzer_init(PyAnalyzer* self, PyObject* args, PyObject* kwds);

PyObject* PyGetServiceErrorCounters(PyAnalyzer* self, ServiceErrorCounters* counters);

PyObject* PyAPIAnalyzeTrace(PyAnalyzer* self, PyObject* args);

PyObject* PyAPIAnalyzeTraceBTrace(PyAnalyzer* self, PyTrace* trace);

PyObject* PyAPIGetServiceErrorCounters(PyAnalyzer* self, PyObject* args);

PyObject* PyAPIGetAllServiceErrorCounters(PyAnalyzer* self);

static PyMethodDef PyAnalyzer_methods[] = {
    {"analyze_trace", (PyCFunction)PyAPIAnalyzeTrace, METH_VARARGS, "Analyze trace"},
    {"get_service_error_counters", (PyCFunction)PyAPIGetServiceErrorCounters, METH_VARARGS, "Get service error counters by service name"},
    {"get_all_service_error_counters", (PyCFunction)PyAPIGetAllServiceErrorCounters, METH_NOARGS, "Get all found service error counters"},
    {NULL}
};

static PyTypeObject PyAnalyzerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.Analyzer",
    .tp_doc = "Analyzer objects",
    .tp_basicsize = sizeof(PyAnalyzer),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyAnalyzer_new,
    .tp_init = (initproc)PyAnalyzer_init,
    .tp_dealloc = (destructor)PyAnalyzer_dealloc,
    .tp_methods = PyAnalyzer_methods,
};

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC PyInit_otelanalyzer(void);