#pragma once

#include <Python.h>

#include "py_structs.h"
#include "py_translator.h"

#include "../analyzer.h"
#include "../structures/trace.h"

typedef struct PyPlugin {
    PyObject* pyPlugin;
    struct PyPlugin* next;
} PyPlugin;


typedef struct {
    PyObject_HEAD
    PyPlugin* pyPluginsHead;
} pyPluginManager;

PyObject* RelevantCounters(Analyzer* a, Trace* t);

void InitPyPluginManager(pyPluginManager* self);

void FreePyPluginManager(pyPluginManager* self);

void PyPluginManager_dealloc(pyPluginManager* self);

void FreeAllPlugins(PyPlugin* head);

PyObject* PyPluginManager_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

int PyPluginManager_init(pyPluginManager* self, PyObject* args, PyObject* kwds);

void RunPyPlugins(pyPluginManager* self, Analyzer* a, Trace* t);

PyObject* PyAPIAddPyPlugin(pyPluginManager* self, PyObject* args);

extern PyMethodDef PyPluginManager_methods[2];

extern PyTypeObject PyPluginManagerType;
