#pragma once

#include <Python.h>
#include <structmember.h>
#include "../analyzer.h"
#include "py_structs.h"

typedef struct {
    PyObject_HEAD
    Analyzer* analyzer;
    // TODO: what type? mb PyObject
    PyObject* pluginManager;

} PyAnalyzer;

void PyAnalyzer_dealloc(PyAnalyzer* self);

PyObject* PyAnalyzer_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PyAnalyzer_init(PyAnalyzer* self, PyObject* args, PyObject* kwds);

PyObject* PyAPIAnalyzeTrace(PyAnalyzer* self, PyObject* args);

PyObject* PyAPIAnalyzeTraceBTrace(PyAnalyzer* self, PyTrace* trace);

PyObject* PyAPIGetServiceErrorCounters(PyAnalyzer* self, PyObject* args);

PyObject* PyAPIGetServiceErrorCountersObj(PyAnalyzer* self, PyObject* args);

PyObject* PyAPIGetAllServiceErrorCounters(PyAnalyzer* self);

PyObject* PyAPIGetAllServiceErrorCountersObj(PyAnalyzer* self);

PyObject* PyAPIGetServiceObj(PyAnalyzer* self, PyObject* args);

PyObject* PyAPIGetAllServiceObj(PyAnalyzer* self);

extern PyMethodDef PyAnalyzer_methods[9];

extern PyMemberDef PyAnalyzer_members[2];

extern PyTypeObject PyAnalyzerType;

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
