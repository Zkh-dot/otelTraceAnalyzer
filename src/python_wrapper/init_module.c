#include "py_analyzer.h"
#include "py_structs.h"


PyMODINIT_FUNC PyInit_otelanalyzer(void) {
    PyObject* m;
    PyObject* type_obj = (PyObject*)&PyAnalyzerType;

    if (PyType_Ready(&PyAnalyzerType) < 0) {
        return NULL;
    }

    if (PyType_Ready(&PyTraceType) < 0) {
        return NULL;
    }

    // if (PyType_Ready(&PyCountersType) < 0) {
    //     return NULL;
    // }

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
    // PyObject* counters_obj = (PyObject*)&PyCountersType;
    // if(PyModule_AddObject(m, "Counters", counters_obj) < 0) {
    //     Py_DECREF(m);
    //     return NULL;
    // }

    return m;
}