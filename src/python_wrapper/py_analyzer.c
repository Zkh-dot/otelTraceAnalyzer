#include "py_analyzer.h"
#include "py_translator.h"
#include "py_plugin_manager.h"

void PyAnalyzer_dealloc(PyAnalyzer* self) {
    FreeAnalyzer(self->analyzer);
    FreePyPluginManager((pyPluginManager*)self->pluginManager);
    // Py_DECREF(self->pluginManager);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyAnalyzer_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyAnalyzer* self;
    self = (PyAnalyzer*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->analyzer = (Analyzer*)malloc(sizeof(Analyzer));
        if (self->analyzer == NULL) {
            Py_DECREF(self);
            return NULL;
        }
        InitAnalyzer(self->analyzer);

        self->pluginManager = PyType_GenericAlloc(&PyPluginManagerType, 0);
        if (self->pluginManager == NULL) {
            Py_DECREF(self);
            return NULL;
        }
        InitPyPluginManager((pyPluginManager*)self->pluginManager);
    }
    return (PyObject*)self;
}

int PyAnalyzer_init(PyAnalyzer* self, PyObject* args, PyObject* kwds) {
    return 0;
}

void PyAnalyzeTrace(PyAnalyzer* self, Trace* trace) {
    ParceTrace(self->analyzer, trace);
    RunPlugins(self->analyzer, trace);
    RunPyPlugins((pyPluginManager*)self->pluginManager, self->analyzer, trace);
    if(!self->analyzer->storeTraces)
        FreeTrace(trace);
}

PyObject* PyAPIAnalyzeTrace(PyAnalyzer* self, PyObject* args) {
    const char* traceString;
    const char* serviceName;
    const char* traceId;
    if (!PyArg_ParseTuple(args, "sss", &traceString, &serviceName, &traceId)) {
        return NULL;
    }
    Trace* trace = (Trace*)malloc(sizeof(Trace));
    InitTrace(trace, traceString, serviceName, traceId);
    PyAnalyzeTrace(self, trace);
    // TODO: maybe, but better not
    // if(self->analyzer->storeTraces)
    //     _rupdateTrace(trace);
    Py_RETURN_NONE;
}

PyObject* PyAPIAnalyzeTraceBTrace(PyAnalyzer* self, PyTrace* trace) {
    _updateTrace(trace);
    PyAnalyzeTrace(self, trace->_trace);
    if(self->analyzer->storeTraces)
        _rupdateTrace(trace);
    Py_RETURN_NONE;
}

PyObject* PyAPIGetServiceErrorCounters(PyAnalyzer* self, PyObject* args) {
    const char* serviceName;
    if (!PyArg_ParseTuple(args, "s", &serviceName)) {
        return NULL;
    }
    ServiceErrorCounters* counters = APIGetServiceErrorCounters(self->analyzer, serviceName);
    PyObject* dict = Counters2Dict(counters);
    printf("%d\n", PyDict_Check(dict)); // to delete
    if(!PyDict_Check(dict))
        return NULL;
    return dict;
}

PyObject* PyAPIGetServiceErrorCountersObj(PyAnalyzer* self, PyObject* args) {
    const char* serviceName;
    if (!PyArg_ParseTuple(args, "s", &serviceName)) {
        return NULL;
    }
    ServiceErrorCounters* counters = APIGetServiceErrorCounters(self->analyzer, serviceName);
    PyCounters* pyCounters = (PyCounters*)PyType_GenericAlloc(&PyCountersType, 0);
    Py_INCREF(pyCounters);
    setCounters4PyCounters(pyCounters, counters);
    return (PyObject*)pyCounters;
}

PyObject* PyAPIGetAllServiceErrorCounters(PyAnalyzer* self) {
    CountersArr* countersArr = APIGetAllServiceErrorCounters(self->analyzer);
    PyObject* dict = PyDict_New();
    for(int i = 0; i < countersArr->errorCountersCount; i++) {
        PyObject* counters = Counters2Dict(countersArr->errorCounters[i]);
        PyDict_SetItemString(dict, countersArr->errorCounters[i]->serviceName, counters);
    }
    return dict;
}

PyObject* PyAPIGetAllServiceErrorCountersObj(PyAnalyzer* self) {
    CountersArr* countersArr = APIGetAllServiceErrorCounters(self->analyzer);
    PyObject* dict = PyDict_New();
    for(int i = 0; i < countersArr->errorCountersCount; i++) {
        PyObject* tmpCounters = PyType_GenericAlloc(&PyCountersType, 0);
        Counters2PyCounters((PyCounters*)tmpCounters, countersArr->errorCounters[i]);
        PyDict_SetItemString(
            dict,
            countersArr->errorCounters[i]->serviceName,
            tmpCounters
        );
    }
    return dict;
}

PyObject* PyAPIGetServiceObj(PyAnalyzer* self, PyObject* args) {
    const char* serviceName;
    if (!PyArg_ParseTuple(args, "s", &serviceName)) {
        return NULL;
    }
    Service* service = GetAddService(self->analyzer, serviceName);
    PyService* pyService = (PyService*)PyType_GenericAlloc(&PyServiceType, 0);
    Py_INCREF(pyService);
    setService4PyService(pyService, service);
    return (PyObject*)pyService;
}

PyObject* PyAPIGetAllServiceObj(PyAnalyzer* self) {
    PyObject* list = PyList_New(self->analyzer->serviceCount);
    size_t iter = 0;
    int counter = 0;
    void *item;
    struct StringToService* e;
    while((hashmap_iter(self->analyzer->serviceMap, &iter, &item))) {
        e = (struct StringToService*)item;
        PyService* pyService = (PyService*)PyType_GenericAlloc(&PyServiceType, 0);
        Py_INCREF(pyService);
        setService4PyService(pyService, e->service);
        PyList_SetItem(list, counter, (PyObject*)pyService);
        counter++;
    }
    return list;
}

PyMemberDef PyAnalyzer_members[] = {
    {"plg_manager", T_OBJECT_EX, offsetof(PyAnalyzer, pluginManager), 0, "Plugin manager object"},
    {NULL}
};

PyMethodDef PyAnalyzer_methods[] = {
    {"analyze", (PyCFunction)PyAPIAnalyzeTrace, METH_VARARGS, "Analyze trace"},
    {"analyze_btrace", (PyCFunction)PyAPIAnalyzeTraceBTrace, METH_O, "Analyze trace by trace object"},
    {"get_counters", (PyCFunction)PyAPIGetServiceErrorCounters, METH_VARARGS, "Get service error counters by service name"},
    {"get_counters_obj", (PyCFunction)PyAPIGetServiceErrorCountersObj, METH_VARARGS, "Get service error counters by service name in py obj in py object"},
    {"get_all_counters", (PyCFunction)PyAPIGetAllServiceErrorCounters, METH_NOARGS, "Get all found service error counters"},
    {"get_all_counters_obj", (PyCFunction)PyAPIGetAllServiceErrorCountersObj, METH_NOARGS, "Get all found service error counters in py object"},
    {"get_service", (PyCFunction)PyAPIGetServiceObj, METH_VARARGS, "Get service by service name"},
    {"get_all_services", (PyCFunction)PyAPIGetAllServiceObj, METH_NOARGS, "Get all found services"},
    {NULL}
};

PyTypeObject PyAnalyzerType = {
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
    .tp_members = PyAnalyzer_members,
};