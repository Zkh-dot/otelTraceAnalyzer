#include "py_structs.h"
#include <structmember.h>

// counters
void PyCounters_dealloc(PyCounters* self) {
    if(self->ownsStatusCounter)
        FreeServiceErrorCounters(self->_statusCounter);
    Py_DECREF(self->myBadTraceExamples);
    Py_DECREF(self->notmyBadTraceExamples);
    Py_DECREF(self->myExamplesCount);
    Py_DECREF(self->notmyExamplesCount);
    Py_DECREF(self->serviceName);
    Py_DECREF(self->badTraceCount);
    Py_DECREF(self->mySpanCount);
    Py_DECREF(self->notmySpanCount);
    Py_DECREF(self->traceCount);
    Py_DECREF(self->inTraceSpanCount);
    Py_DECREF(self->statusCounter);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyCounters_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyCounters* self;
    self = (PyCounters*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->myBadTraceExamples = PyList_New(0);
        self->_statusCounter = NULL;
        self->ownsStatusCounter = false;
        self->statusCounter = PyDict_New();
        self->notmyBadTraceExamples = PyList_New(0);
        self->myExamplesCount = PyLong_FromLong(0);
        self->notmyExamplesCount = PyLong_FromLong(0);
        self->serviceName = PyUnicode_FromString("");
        self->badTraceCount = PyLong_FromLong(0);
        self->mySpanCount = PyLong_FromLong(0);
        self->notmySpanCount = PyLong_FromLong(0);
        self->traceCount = PyLong_FromLong(0);
        self->inTraceSpanCount = PyLong_FromLong(0);
    }
    return (PyObject*)self;
}

int PyCounters_init(PyCounters* self, PyObject* args, PyObject* kwds) {
    return 0;
}

static void _updatePyCounterFields(PyCounters* self) {
    self->badTraceCount =       Py_BuildValue("i", self->_statusCounter->badTraceCount);
    self->mySpanCount =         Py_BuildValue("i", self->_statusCounter->mySpanCount);
    self->notmySpanCount =      Py_BuildValue("i", self->_statusCounter->notmySpanCount);
    self->traceCount =          Py_BuildValue("i", self->_statusCounter->traceCount);
    self->inTraceSpanCount =    Py_BuildValue("i", self->_statusCounter->inTraceSpanCount);
    self->myExamplesCount =     Py_BuildValue("i", self->_statusCounter->myExamplesCount);
    self->notmyExamplesCount =  Py_BuildValue("i", self->_statusCounter->notmyExamplesCount);

    self->myBadTraceExamples = PyList_New(0);
    self->notmyBadTraceExamples = PyList_New(0);
    self->statusCounter = PyDict_New();

    for (int i = 0; i < self->_statusCounter->myExamplesCount; i++) {
        PyList_Append(
            self->myBadTraceExamples,
            Py_BuildValue(
                "s",
                self->_statusCounter->myBadTraceExamples[i],
                strlen(self->_statusCounter->myBadTraceExamples[i])
            )
        );
    }
    for (int i = 0; i < self->_statusCounter->notmyExamplesCount; i++) {
        PyList_Append(self->notmyBadTraceExamples, Py_BuildValue("s", self->_statusCounter->notmyBadTraceExamples[i], strlen(self->_statusCounter->notmyBadTraceExamples[i])));
    }
    
    for(int i = 0; i < TraceOk + 1; i++) {
        PyDict_SetItem(
            self->statusCounter,
            Py_BuildValue("s", traceStatusMessage[i], strlen(traceStatusMessage[i])),
            Py_BuildValue("i", self->_statusCounter->statusCounter[i])
        );
    }
}

void _updateCCounter(PyCounters* self) {
    self->_statusCounter->badTraceCount =       (int)PyLong_AsLong(self->badTraceCount);
    self->_statusCounter->mySpanCount =         (int)PyLong_AsLong(self->mySpanCount);
    self->_statusCounter->notmySpanCount =      (int)PyLong_AsLong(self->notmySpanCount);
    self->_statusCounter->traceCount =          (int)PyLong_AsLong(self->traceCount);
    self->_statusCounter->inTraceSpanCount =    (int)PyLong_AsLong(self->inTraceSpanCount);
    self->_statusCounter->myExamplesCount =     (int)PyLong_AsLong(self->myExamplesCount);
    self->_statusCounter->notmyExamplesCount =  (int)PyLong_AsLong(self->notmyExamplesCount);

    for (int i = 0; i < self->_statusCounter->myExamplesCount; i++) {
        self->_statusCounter->myBadTraceExamples[i] = (char*)PyUnicode_AsUTF8(PyList_GetItem(self->myBadTraceExamples, i));
    }
    for (int i = 0; i < self->_statusCounter->notmyExamplesCount; i++) {
        self->_statusCounter->notmyBadTraceExamples[i] = (char*)PyUnicode_AsUTF8(PyList_GetItem(self->notmyBadTraceExamples, i));
    }
    for (int i = 0; i < TraceOk + 1; i ++) {
        self->_statusCounter->statusCounter[i] = (int)PyLong_AsLong(PyDict_GetItemString(self->statusCounter, traceStatusMessage[i]));
    }
}

void _updatePyCounter(PyCounters* self, ServiceErrorCounters* counters) {
    self->_statusCounter = counters;
    self->ownsStatusCounter = false;
    _updatePyCounterFields(self);
}

PyMethodDef PyCounters_methods[] = {
    {NULL}
};

PyMemberDef PyCounters_members[] = {
    {"myBadTraceExamples", T_OBJECT_EX, offsetof(PyCounters, myBadTraceExamples), 0, "My bad trace examples"},
    {"notmyBadTraceExamples", T_OBJECT_EX, offsetof(PyCounters, notmyBadTraceExamples), 0, "Not my bad trace examples"},
    {"myExamplesCount", T_OBJECT_EX, offsetof(PyCounters, myExamplesCount), 0, "My examples count"},
    {"notmyExamplesCount", T_OBJECT_EX, offsetof(PyCounters, notmyExamplesCount), 0, "Not my examples count"},
    {"serviceName", T_OBJECT_EX, offsetof(PyCounters, serviceName), 0, "Service name"},
    {"badTraceCount", T_OBJECT_EX, offsetof(PyCounters, badTraceCount), 0, "Bad trace count"},
    {"mySpanCount", T_OBJECT_EX, offsetof(PyCounters, mySpanCount), 0, "My span count"},
    {"notmySpanCount", T_OBJECT_EX, offsetof(PyCounters, notmySpanCount), 0, "Not my span count"},
    {"traceCount", T_OBJECT_EX, offsetof(PyCounters, traceCount), 0, "Trace count"},
    {"inTraceSpanCount", T_OBJECT_EX, offsetof(PyCounters, inTraceSpanCount), 0, "In trace span count"},
    {"statusCounters", T_OBJECT_EX, offsetof(PyCounters, statusCounter), 0, "Trace count"},
    {NULL}
};

PyTypeObject PyCountersType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.Counters",
    .tp_doc = "Counters objects",
    .tp_basicsize = sizeof(PyCounters),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyCounters_new,
    .tp_init = (initproc)PyCounters_init,
    .tp_dealloc = (destructor)PyCounters_dealloc,
    .tp_methods = PyCounters_methods,
    .tp_members = PyCounters_members,
};

// span
void PySpan_dealloc(PySpan* self) {
    Py_XDECREF(self->spanId);
    Py_XDECREF(self->serviceName);
    Py_XDECREF(self->parentSpanId);
    Py_XDECREF(self->traceId);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PySpan_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PySpan* self;
    self = (PySpan*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->_span = NULL;
        self->spanId = PyUnicode_FromString("");
        self->serviceName = PyUnicode_FromString("");
        self->parentSpanId = PyUnicode_FromString("");
        self->traceId = PyUnicode_FromString("");
        if (
            self->spanId == NULL ||
            self->serviceName == NULL ||
            self->parentSpanId == NULL ||
            self->traceId == NULL
        ) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}

int PySpan_init(PySpan* self, PyObject* args, PyObject* kwds) {
    return 0;
}

static int replace_object_field(PyObject** field, PyObject* value) {
    if (value == NULL) {
        return -1;
    }
    Py_XDECREF(*field);
    *field = value;
    return 0;
}

static PyObject* py_string_or_empty(const char* value) {
    return PyUnicode_FromString(value != NULL ? value : "");
}

void _updatePySpan(PySpan* self, Span* span) {
    self->_span = span;
    if (replace_object_field(&self->spanId, py_string_or_empty(span != NULL ? span->spanId : NULL)) < 0) {
        return;
    }
    if (replace_object_field(&self->serviceName, py_string_or_empty(span != NULL ? span->serviceName : NULL)) < 0) {
        return;
    }
    if (replace_object_field(&self->parentSpanId, py_string_or_empty(span != NULL ? span->parentSpanId : NULL)) < 0) {
        return;
    }
    if (replace_object_field(&self->traceId, py_string_or_empty(span != NULL ? span->traceId : NULL)) < 0) {
        return;
    }
}

PyMethodDef PySpan_methods[] = {
    {NULL}
};

PyMemberDef PySpan_members[] = {
    {"spanId", T_OBJECT_EX, offsetof(PySpan, spanId), 0, "Span ID"},
    {"serviceName", T_OBJECT_EX, offsetof(PySpan, serviceName), 0, "Service name"},
    {"parentSpanId", T_OBJECT_EX, offsetof(PySpan, parentSpanId), 0, "Parent span ID"},
    {"traceId", T_OBJECT_EX, offsetof(PySpan, traceId), 0, "Trace ID"},
    {NULL}
};

PyTypeObject PySpanType = {
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
    .tp_members = PySpan_members,
};

PyMethodDef PyTrace_methods[] = {
    {NULL}
};


// trace
void PyTrace_dealloc(PyTrace* self) {
    Py_XDECREF(self->traceString);
    Py_XDECREF(self->serviceName);
    Py_XDECREF(self->traceId);
    Py_XDECREF(self->spansCount);
    Py_XDECREF(self->spansList);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyTrace_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyTrace* self;
    self = (PyTrace*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->traceString = PyUnicode_FromString("");
        self->serviceName = PyUnicode_FromString("");
        self->traceId = PyUnicode_FromString("");
        self->spansCount = PyLong_FromLong(0);
        self->spansList = PyList_New(0);
        if (
            self->traceString == NULL ||
            self->serviceName == NULL ||
            self->traceId == NULL ||
            self->spansCount == NULL ||
            self->spansList == NULL
        ) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}

int PyTrace_init(PyTrace* self, PyObject* args, PyObject* kwds) {
    return 0;
}

void _updateCTrace(PyTrace* self) {
    self->_trace = (Trace*)malloc(sizeof(Trace));
    char* traceString = (char*)PyUnicode_AsUTF8(self->traceString);
    char* serviceName = (char*)PyUnicode_AsUTF8(self->serviceName);
    char* traceId = (char*)PyUnicode_AsUTF8(self->traceId);
    InitTrace(self->_trace, traceString, serviceName, traceId);
}

void _updatePyTrace(PyTrace* self, Trace* trace) {
    self->_trace = trace;
    if (self->_trace == NULL) {
        return;
    }

    PyObject* serviceName = py_string_or_empty(self->_trace->serviceName);
    PyObject* traceId = py_string_or_empty(self->_trace->traceId);
    PyObject* spansCount = PyLong_FromLong(self->_trace->spansCount);
    PyObject* spansList = PyList_New(0);
    if (serviceName == NULL || traceId == NULL || spansCount == NULL || spansList == NULL) {
        Py_XDECREF(serviceName);
        Py_XDECREF(traceId);
        Py_XDECREF(spansCount);
        Py_XDECREF(spansList);
        return;
    }

    if (PySpanType.tp_dict == NULL && PyType_Ready(&PySpanType) < 0) {
        Py_DECREF(serviceName);
        Py_DECREF(traceId);
        Py_DECREF(spansCount);
        Py_DECREF(spansList);
        return;
    }

    for (int i = 0; i < self->_trace->spansCount; i++) {
        PySpan* pySpan = (PySpan*)PyObject_CallObject((PyObject*)&PySpanType, NULL);
        if (pySpan == NULL) {
            Py_DECREF(serviceName);
            Py_DECREF(traceId);
            Py_DECREF(spansCount);
            Py_DECREF(spansList);
            return;
        }
        _updatePySpan(pySpan, self->_trace->spans[i]);
        if (PyList_Append(spansList, (PyObject*)pySpan) < 0) {
            Py_DECREF(pySpan);
            Py_DECREF(serviceName);
            Py_DECREF(traceId);
            Py_DECREF(spansCount);
            Py_DECREF(spansList);
            return;
        }
        Py_DECREF(pySpan);
    }

    replace_object_field(&self->serviceName, serviceName);
    replace_object_field(&self->traceId, traceId);
    replace_object_field(&self->spansCount, spansCount);
    replace_object_field(&self->spansList, spansList);
}

PyMemberDef PyTrace_members[] = {
    {"traceString", T_OBJECT_EX, offsetof(PyTrace, traceString), 0, "Trace string"},
    {"serviceName", T_OBJECT_EX, offsetof(PyTrace, serviceName), 0, "Service name"},
    {"traceId", T_OBJECT_EX, offsetof(PyTrace, traceId), 0, "Trace ID"},
    {"spansCount", T_OBJECT_EX, offsetof(PyTrace, spansCount), 0, "Spans count"},
    {"spansList", T_OBJECT_EX, offsetof(PyTrace, spansList), 0, "Spans list"},
    {NULL}
};

PyTypeObject PyTraceType = {
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
    .tp_members = PyTrace_members,
};


// service
void PyService_dealloc(PyService* self) {
    Py_DECREF(self->errorCounters);
    Py_DECREF(self->serviceName);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyService_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyService* self;
    self = (PyService*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->errorCounters = (PyCounters*)PyObject_CallObject((PyObject*)&PyCountersType, NULL);
        self->serviceName = PyUnicode_FromString("");
    }
    return (PyObject*)self;
}

int PyService_init(PyService* self, PyObject* args, PyObject* kwds) {
    return 0;    
}

void _updateCService(PyService* self) {
    self->_service = (Service*)malloc(sizeof(Service));
    char* serviceName = (char*)PyUnicode_AsUTF8(self->serviceName);
    InitService(self->_service, serviceName);
}

void _updatePyService(PyService* self, Service* service) {
    PyObject* serviceName = PyUnicode_FromString(service->serviceName);
    if (serviceName == NULL) {
        return;
    }

    PyCounters* errorCounters = (PyCounters*)PyObject_CallObject((PyObject*)&PyCountersType, NULL);
    if (errorCounters == NULL) {
        Py_DECREF(serviceName);
        return;
    }

    ServiceErrorCounters* countersCopy = (ServiceErrorCounters*)malloc(sizeof(ServiceErrorCounters));
    if (countersCopy == NULL) {
        Py_DECREF(serviceName);
        Py_DECREF(errorCounters);
        PyErr_NoMemory();
        return;
    }
    InitServiceErrorCounters(countersCopy);
    CopyServiceErrorCounters(countersCopy, service->errorCounters);
    _updatePyCounter(errorCounters, countersCopy);
    errorCounters->ownsStatusCounter = true;

    self->_service = service;
    Py_DECREF(self->serviceName);
    self->serviceName = serviceName;
    Py_DECREF(self->errorCounters);
    self->errorCounters = errorCounters;
}

PyObject* PyService_is_ok(PyService* self, PyObject* Py_UNUSED(ignored)) {
    if (self->errorCounters == NULL || self->errorCounters->_statusCounter == NULL) {
        Py_RETURN_TRUE;
    }
    for (int i = 0; i < TraceOk; i++) {
        if (self->errorCounters->_statusCounter->statusCounter[i] != 0) {
            Py_RETURN_FALSE;
        }
    }
    Py_RETURN_TRUE;
}

PyMethodDef PyService_methods[] = {
    {"is_ok", (PyCFunction)PyService_is_ok, METH_NOARGS, "Return whether the service has no non-OK trace statuses"},
    {NULL}
};

PyMemberDef PyService_members[] = {
    {"errorCounters", T_OBJECT_EX, offsetof(PyService, errorCounters), 0, "Error counters"},
    {"serviceName", T_OBJECT_EX, offsetof(PyService, serviceName), 0, "Service name"},
    {NULL}
};

PyTypeObject PyServiceType = {
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
        .tp_members = PyService_members,
};
