#include "py_analyzer.h"

static void PyAnalyzer_dealloc(PyAnalyzer* self) {
    FreeAnalyzer(self->analyzer);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyAnalyzer_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyAnalyzer* self;
    self = (PyAnalyzer*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->analyzer = (Analyzer*)malloc(sizeof(Analyzer));
        if (self->analyzer == NULL) {
            Py_DECREF(self);
            return NULL;
        }
        InitAnalyzer(self->analyzer);
    }
    return (PyObject*)self;
}

static int PyAnalyzer_init(PyAnalyzer* self, PyObject* args, PyObject* kwds) {
    return 0;
}

PyObject* PyAPIAnalyzeTrace(PyAnalyzer* self, PyObject* args) {
    const char* traceString;
    const char* serviceName;
    const char* traceId;
    if (!PyArg_ParseTuple(args, "sss", &traceString, &serviceName, &traceId)) {
        return NULL;
    }
    APIAnalyzeTrace(self->analyzer, traceString, serviceName, traceId);
    Py_RETURN_NONE;
}

PyObject* PyAPIAnalyzeTraceBTrace(PyAnalyzer* self, PyTrace* trace) {
    _updateTrace(trace);
    AnalyzeTrace(self->analyzer, trace->_trace);
    _rupdateTrace(trace);
    Py_RETURN_NONE;
}

PyObject* PyGetServiceErrorCounters(PyAnalyzer* self, ServiceErrorCounters* counters) {
    PyObject* dict = PyDict_New();
    for(int i = 0; i < TraceOk; i++) {
        PyDict_SetItemString(dict, traceStatusMessage[i], PyLong_FromLong(counters->statusCounter[i]));
    }
    PyObject* myExamples = PyList_New(counters->myExamplesCount);
    for(int i = 0; i < counters->myExamplesCount; i++) {
        PyList_SetItem(myExamples, i, PyUnicode_FromString(counters->myBadTraceExamples[i]));
    }
    PyObject* notmyExamples = PyList_New(counters->notmyExamplesCount);
    for(int i = 0; i < counters->notmyExamplesCount; i++) {
        PyList_SetItem(notmyExamples, i, PyUnicode_FromString(counters->notmyBadTraceExamples[i]));
    }
    PyDict_SetItemString(dict, "myExamples", myExamples);
    PyDict_SetItemString(dict, "notmyExamples", notmyExamples);
    PyDict_SetItemString(dict, "badTraceCount", PyLong_FromLong(counters->badTraceCount));
    PyDict_SetItemString(dict, "mySpanCount", PyLong_FromLong(counters->mySpanCount));
    PyDict_SetItemString(dict, "traceCount", PyLong_FromLong(counters->traceCount));
    return dict;
}

PyObject* PyAPIGetServiceErrorCounters(PyAnalyzer* self, PyObject* args) {
    const char* serviceName;
    if (!PyArg_ParseTuple(args, "s", &serviceName)) {
        return NULL;
    }
    ServiceErrorCounters* counters = APIGetServiceErrorCounters(self->analyzer, serviceName);
    return PyGetServiceErrorCounters(self, counters);
}

PyObject* PyAPIGetServiceErrorCountersObj(PyAnalyzer* self, PyObject* args) {
    const char* serviceName;
    if (!PyArg_ParseTuple(args, "s", &serviceName)) {
        return NULL;
    }
    ServiceErrorCounters* counters = APIGetServiceErrorCounters(self->analyzer, serviceName);
    return (PyObject*)Counters2PyCounters(counters);
}

PyObject* PyAPIGetAllServiceErrorCounters(PyAnalyzer* self) {
    CountersArr* countersArr = APIGetAllServiceErrorCounters(self->analyzer);
    PyObject* dict = PyDict_New();
    for(int i = 0; i < countersArr->errorCountersCount; i++) {
        PyObject* counters = PyGetServiceErrorCounters(self, countersArr->errorCounters[i]);
        PyDict_SetItemString(dict, countersArr->errorCounters[i]->serviceName, counters);
    }
    return dict;
}

PyObject* PyAPIGetAllServiceErrorCountersObj(PyAnalyzer* self) {
    CountersArr* countersArr = APIGetAllServiceErrorCounters(self->analyzer);
    PyObject* dict = PyDict_New();
    for(int i = 0; i < countersArr->errorCountersCount; i++) {
        PyDict_SetItemString(
            dict,
            countersArr->errorCounters[i]->serviceName,
            Counters2PyCounters(countersArr->errorCounters[i])
        );
    }
    return dict;
}

PyMethodDef PyAnalyzer_methods[] = {
    {"analyze", (PyCFunction)PyAPIAnalyzeTrace, METH_VARARGS, "Analyze trace"},
    {"analyze_btrace", (PyCFunction)PyAPIAnalyzeTraceBTrace, METH_O, "Analyze trace by trace object"},
    {"get_counters", (PyCFunction)PyAPIGetServiceErrorCounters, METH_VARARGS, "Get service error counters by service name"},
    {"get_counters_obj", (PyCFunction)PyAPIGetServiceErrorCountersObj, METH_VARARGS, "Get service error counters by service name in py obj in py object"},
    {"get_all_counters", (PyCFunction)PyAPIGetAllServiceErrorCounters, METH_NOARGS, "Get all found service error counters"},
    {"get_all_counters_obj", (PyCFunction)PyAPIGetAllServiceErrorCountersObj, METH_NOARGS, "Get all found service error counters in py object"},
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
};