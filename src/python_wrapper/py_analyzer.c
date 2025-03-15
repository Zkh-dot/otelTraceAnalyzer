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

PyMODINIT_FUNC PyInit_otelanalyzer(void) {
    PyObject* m;
    PyObject* type_obj = (PyObject*)&PyAnalyzerType;

    if (PyType_Ready(&PyAnalyzerType) < 0) {
        return NULL;
    }

    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "otelanalyzer",
        "module to analyze traces on OpenTelemetry format",
        -1,
        NULL,
    };

    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }

    if (PyModule_AddObject(m, "Analyzer", type_obj) < 0) {
        Py_DECREF(m);
        return NULL;
    }
    PyObject* trace_obj = (PyObject*)&PyTraceType;
    if(PyModule_AddObject(m, "Trace", trace_obj) < 0) {
        Py_DECREF(m);
        return NULL;
    }

    return m;
}