#include "py_plugin_manager.h"

void FreeAllPlugins(PyPlugin* firstPyPlugin) {
    PyPlugin* tmp = firstPyPlugin;
    while(tmp != NULL) {
        PyPlugin* next = tmp->next;
        free(tmp);
        tmp = next;
    }
}

void InitPyPluginManager(pyPluginManager* self) {
    self->pyPluginsHead = NULL;
}

void FreePyPluginManager(pyPluginManager* self) {
    FreeAllPlugins(self->pyPluginsHead);
}

PyObject* RelevantCounters(Analyzer* a, Trace* t) {
    PyObject* dict = PyDict_New();
    for(int i = 0; i < t->spansCount; i++) {
        PyCounters* pyCounters = (PyCounters*)PyType_GenericAlloc(&PyCountersType, 0);
        setCounters4PyCounters(pyCounters, APIGetServiceErrorCounters(a, t->spans[i]->serviceName));
        PyDict_SetItemString(dict, t->spans[i]->serviceName, (PyObject*)pyCounters);
    }
    return dict;
}

void UpdateRelevantCounters(Analyzer* a, PyObject* dict) {
    PyObject* key, *value;
    Py_ssize_t pos = 0;
    while(PyDict_Next(dict, &pos, &key, &value)) {
        _rupdateCounter((PyCounters*)value);
    }
}

void FreeRelevantCounters(PyObject* list) {
    PyObject* key, *value;
    Py_ssize_t pos = 0;
    while(PyDict_Next(list, &pos, &key, &value)) {
        Py_DECREF(value);
    }
}

void RunPyPlugins(pyPluginManager* self, Analyzer* a, Trace* t) {
    PyPlugin* tmp = self->pyPluginsHead;
    PyObject* relevantCounters = RelevantCounters(a, t);
    PyObject* tmpTrace = PyType_GenericAlloc(&PyTraceType, 0);
    while(tmp != NULL) {
        PyObject* pyPlugin = tmp->pyPlugin;
        tmpTrace = Trace2PyTrace((PyTrace*)tmpTrace, t);
        PyObject* result = PyObject_CallFunction(
            pyPlugin,
            "OO",
            relevantCounters,
            tmpTrace
        );

        if (PyDict_Check(result)) {
            UpdateRelevantCounters(a, result);
        }
        
        tmp = tmp->next;
        Py_DECREF(result);
    }
    Py_DECREF(relevantCounters);
}

void PyPluginManager_dealloc(pyPluginManager* self) {
    FreeAllPlugins(self->pyPluginsHead);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyPluginManager_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pyPluginManager* self;
    self = (pyPluginManager*)type->tp_alloc(type, 0);
    if(self != NULL) {
        InitPyPluginManager(self);
    }
    return (PyObject*)self;
}

int PyPluginManager_init(pyPluginManager* self, PyObject* args, PyObject* kwds) {
    return 0;
}

PyObject* PyAPIAddPyPlugin(pyPluginManager* self, PyObject* args) {
    PyObject* pyPlugin;
    if(!PyArg_ParseTuple(args, "O", &pyPlugin)) {
        return NULL;
    }
    PyPlugin* newPyPlugin = (PyPlugin*)malloc(sizeof(PyPlugin));
    newPyPlugin->pyPlugin = pyPlugin;
    newPyPlugin->next = self->pyPluginsHead;
    self->pyPluginsHead = newPyPlugin;
    Py_RETURN_NONE;
}

PyMethodDef PyPluginManager_methods[] = {
    {"add_plugin", (PyCFunction)PyAPIAddPyPlugin, METH_VARARGS, "Add python plugin"},
    {NULL}
};

PyTypeObject PyPluginManagerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.PluginManager",
    .tp_doc = "Plugin manager objects",
    .tp_basicsize = sizeof(pyPluginManager),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyPluginManager_new,
    .tp_init = (initproc)PyPluginManager_init,
    .tp_dealloc = (destructor)PyPluginManager_dealloc,
    .tp_methods = PyPluginManager_methods,
};