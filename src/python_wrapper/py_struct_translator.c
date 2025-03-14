#include "py_struct_translator.h"

typedef struct {
    PyObject_HEAD
    ServiceErrorCounters* errorCounters;
} PyCounters;

static void PyCounters_dealloc(PyCounters* self) {
    FreeServiceErrorCounters(self->errorCounters);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyCounters_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyCounters* self;
    self = (PyCounters*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->errorCounters = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
        if (self->errorCounters == NULL) {
            Py_DECREF(self);
            return NULL;
        }
        InitServiceErrorCounters(self->errorCounters);
    }
    return (PyObject*)self;
}

static int PyCounters_init(PyCounters* self, PyObject* args, PyObject* kwds) {
    return 0;
}

typedef struct {
    PyObject_HEAD
    Span* span;
} PySpan;

static void PySpan_dealloc(PySpan* self) {
    FreeSpan(self->span);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PySpan_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PySpan* self;
    self = (PySpan*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->span = (Span*)malloc(sizeof(Span));
        if (self->span == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}

static int PySpan_init(PySpan* self, PyObject* args, PyObject* kwds) {
    const char* spanId;
    const char* serviceName;
    const char* parentSpanId;
    Span* parentSpan;
    if (!PyArg_ParseTuple(args, "sssO", &spanId, &serviceName, &parentSpanId, &parentSpan)) {
        return -1;
    }
    InitSpan(self->span, spanId, serviceName, parentSpanId, parentSpan);
    return 0;
}

typedef struct {
    PyObject_HEAD
    Trace* trace;
} PyTrace;

static void PyTrace_dealloc(PyTrace* self) {
    FreeTrace(self->trace);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyTrace_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyTrace* self;
    self = (PyTrace*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->trace = (Trace*)malloc(sizeof(Trace));
        if (self->trace == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}

static int PyTrace_init(PyTrace* self, PyObject* args, PyObject* kwds) {
    const char* traceString;
    const char* serviceName;
    const char* traceId;
    if (!PyArg_ParseTuple(args, "sss", &traceString, &serviceName, &traceId)) {
        return -1;
    }
    InitTrace(self->trace, traceString, serviceName, traceId);
    return 0;
}

typedef struct {
    PyObject_HEAD
    Service* service;
} PyService;

static void PyService_dealloc(PyService* self) {
    FreeService(self->service);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyService_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyService* self;
    self = (PyService*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->service = (Service*)malloc(sizeof(Service));
        if (self->service == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}

static int PyService_init(PyService* self, PyObject* args, PyObject* kwds) {
    const char* serviceName;
    if (!PyArg_ParseTuple(args, "s", &serviceName)) {
        return -1;
    }
    InitService(self->service, serviceName);
    return 0;
}

static PyMethodDef PyCounters_methods[] = {
    {NULL}
};

static PyTypeObject PyCountersType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.ServiceErrorCounters",
    .tp_doc = "ServiceErrorCounters objects",
    .tp_basicsize = sizeof(PyCounters),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyCounters_new,
    .tp_init = (initproc)PyCounters_init,
    .tp_dealloc = (destructor)PyCounters_dealloc,
    .tp_methods = PyCounters_methods,
};

static PyMethodDef PySpan_methods[] = {
    {NULL}
};

static PyTypeObject PySpanType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.Span",
    .tp_doc = "Span objects",
    .tp_basicsize = sizeof(PySpan),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PySpan_new,
    .tp_init = (initproc)PySpan_init,
    .tp_dealloc = (destructor)PySpan_dealloc,
    .tp_methods = PySpan_methods,
};

static PyMethodDef PyTrace_methods[] = {
    {NULL}
};

static PyTypeObject PyTraceType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.Trace",
    .tp_doc = "Trace objects",
    .tp_basicsize = sizeof(PyTrace),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyTrace_new,
    .tp_init = (initproc)PyTrace_init,
    .tp_dealloc = (destructor)PyTrace_dealloc,
    .tp_methods = PyTrace_methods,
};

static PyMethodDef PyService_methods[] = {
    {NULL}
};

static PyTypeObject PyServiceType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.Service",
    .tp_doc = "Service objects",
    .tp_basicsize = sizeof(PyService),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyService_new,
    .tp_init = (initproc)PyService_init,
    .tp_dealloc = (destructor)PyService_dealloc,
    .tp_methods = PyService_methods,
};

// static PyModuleDef pywrapper_module = {
//     PyModuleDef_HEAD_INIT,
//     .m_name = "pywrapper",
//     .m_doc = "Python wrapper for C structures",
//     .m_size = -1,
// };