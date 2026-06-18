#include "py_structs.h"
#include <structmember.h>

static int replace_object_field(PyObject** field, PyObject* value);

// counters
static int set_dict_item_steal(PyObject* dict, const char* key, PyObject* value) {
    if (value == NULL) {
        return -1;
    }
    int result = PyDict_SetItemString(dict, key, value);
    Py_DECREF(value);
    return result;
}

static PyObject* TraceStatusCounters2Dict(int* statusCounters) {
    PyObject* dict = PyDict_New();
    if (dict == NULL) {
        return NULL;
    }

    for(int i = 0; i < TraceOk + 1; i++) {
        if (set_dict_item_steal(dict, traceStatusMessage[i], PyLong_FromLong(statusCounters[i])) < 0) {
            Py_DECREF(dict);
            return NULL;
        }
    }

    return dict;
}

static PyObject* BadTraceExamples2List(char** examples, int examplesCount) {
    PyObject* list = PyList_New(examplesCount);
    if (list == NULL) {
        return NULL;
    }

    for(int i = 0; i < examplesCount; i++) {
        PyObject* value = PyUnicode_FromString(examples[i] != NULL ? examples[i] : "");
        if (value == NULL) {
            Py_DECREF(list);
            return NULL;
        }
        PyList_SET_ITEM(list, i, value);
    }

    return list;
}

static PyObject* ServiceCounters2Dict(ServiceCounters* counters, int* statusCounters) {
    PyObject* dict = PyDict_New();
    if (dict == NULL) {
        return NULL;
    }

    if (
        set_dict_item_steal(dict, "statusCounters", TraceStatusCounters2Dict(statusCounters)) < 0 ||
        set_dict_item_steal(dict, "myExamples", BadTraceExamples2List(counters->myBadTraceExamples, counters->myExamplesCount)) < 0 ||
        set_dict_item_steal(dict, "notmyExamples", BadTraceExamples2List(counters->notmyBadTraceExamples, counters->notmyExamplesCount)) < 0 ||
        set_dict_item_steal(dict, "myExamplesCount", PyLong_FromLong(counters->myExamplesCount)) < 0 ||
        set_dict_item_steal(dict, "notmyExamplesCount", PyLong_FromLong(counters->notmyExamplesCount)) < 0 ||
        set_dict_item_steal(dict, "mySpanCount", PyLong_FromLong(counters->mySpanCount)) < 0 ||
        set_dict_item_steal(dict, "notmySpanCount", PyLong_FromLong(counters->notmySpanCount)) < 0
    ) {
        Py_DECREF(dict);
        return NULL;
    }

    return dict;
}

static int update_int_from_dict(PyObject* dict, const char* key, int* target) {
    PyObject* value = PyDict_GetItemString(dict, key);
    if (value == NULL) {
        return 0;
    }
    long parsed = PyLong_AsLong(value);
    if (PyErr_Occurred()) {
        return -1;
    }
    *target = (int)parsed;
    return 0;
}

static int update_examples_from_list(char** examples, int* examplesCount, PyObject* list) {
    if (list == NULL) {
        return 0;
    }
    if (!PyList_Check(list)) {
        PyErr_SetString(PyExc_TypeError, "examples must be lists");
        return -1;
    }

    for (int i = 0; i < *examplesCount && i < EXAMPLES_LENGTH; i++) {
        free(examples[i]);
        examples[i] = NULL;
    }

    Py_ssize_t listSize = PyList_Size(list);
    int newCount = listSize < EXAMPLES_LENGTH ? (int)listSize : EXAMPLES_LENGTH;
    for (int i = 0; i < newCount; i++) {
        const char* value = PyUnicode_AsUTF8(PyList_GetItem(list, i));
        if (value == NULL) {
            return -1;
        }
        examples[i] = strdup(value);
    }
    *examplesCount = newCount;
    return 0;
}

static int UpdateServiceCountersFromDict(ServiceCounters* counters, int* statusCounters, PyObject* dict) {
    if (dict == NULL) {
        return 0;
    }
    if (!PyDict_Check(dict)) {
        PyErr_SetString(PyExc_TypeError, "trace counters must be dictionaries");
        return -1;
    }

    PyObject* statusDict = PyDict_GetItemString(dict, "statusCounters");
    if (statusDict != NULL) {
        if (!PyDict_Check(statusDict)) {
            PyErr_SetString(PyExc_TypeError, "statusCounters must be a dictionary");
            return -1;
        }
        for (int i = 0; i < TraceOk + 1; i++) {
            PyObject* value = PyDict_GetItemString(statusDict, traceStatusMessage[i]);
            if (value != NULL) {
                long parsed = PyLong_AsLong(value);
                if (PyErr_Occurred()) {
                    return -1;
                }
                statusCounters[i] = (int)parsed;
            }
        }
    }

    if (
        update_examples_from_list(counters->myBadTraceExamples, &counters->myExamplesCount, PyDict_GetItemString(dict, "myExamples")) < 0 ||
        update_examples_from_list(counters->notmyBadTraceExamples, &counters->notmyExamplesCount, PyDict_GetItemString(dict, "notmyExamples")) < 0 ||
        update_int_from_dict(dict, "mySpanCount", &counters->mySpanCount) < 0 ||
        update_int_from_dict(dict, "notmySpanCount", &counters->notmySpanCount) < 0
    ) {
        return -1;
    }

    return 0;
}

void PyServiceErrorCounters_dealloc(PyServiceErrorCounters* self) {
    if(self->ownsStatusCounter)
        FreeServiceErrorCounters(self->_statusCounter);
    Py_XDECREF(self->myTraces);
    Py_XDECREF(self->notmyTraces);
    Py_XDECREF(self->serviceName);
    Py_XDECREF(self->badTraceCount);
    Py_XDECREF(self->traceCount);
    Py_XDECREF(self->inTraceSpanCount);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyServiceErrorCounters_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyServiceErrorCounters* self;
    self = (PyServiceErrorCounters*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->_statusCounter = NULL;
        self->ownsStatusCounter = false;
        self->myTraces = PyDict_New();
        self->notmyTraces = PyDict_New();
        self->serviceName = PyUnicode_FromString("");
        self->badTraceCount = PyLong_FromLong(0);
        self->traceCount = PyLong_FromLong(0);
        self->inTraceSpanCount = PyLong_FromLong(0);
        if (
            self->myTraces == NULL ||
            self->notmyTraces == NULL ||
            self->serviceName == NULL ||
            self->badTraceCount == NULL ||
            self->traceCount == NULL ||
            self->inTraceSpanCount == NULL
        ) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}

int PyServiceErrorCounters_init(PyServiceErrorCounters* self, PyObject* args, PyObject* kwds) {
    return 0;
}

static void _updatePyCounterFields(PyServiceErrorCounters* self) {
    replace_object_field(&self->myTraces, ServiceCounters2Dict(
        &self->_statusCounter->myTraceServiceCounters,
        self->_statusCounter->myTraceStatusCounter
    ));
    replace_object_field(&self->notmyTraces, ServiceCounters2Dict(
        &self->_statusCounter->notmyTraceServiceCounters,
        self->_statusCounter->notmyTraceStatusCounter
    ));
    replace_object_field(&self->badTraceCount, PyLong_FromLong(self->_statusCounter->badTraceCount));
    replace_object_field(&self->traceCount, PyLong_FromLong(self->_statusCounter->traceCount));
    replace_object_field(&self->inTraceSpanCount, PyLong_FromLong(self->_statusCounter->inTraceSpanCount));
    replace_object_field(&self->serviceName, PyUnicode_FromString(self->_statusCounter->serviceName != NULL ? self->_statusCounter->serviceName : ""));
}

void _updateCCounter(PyServiceErrorCounters* self) {
    if (self->_statusCounter == NULL) {
        return;
    }
    self->_statusCounter->badTraceCount = (int)PyLong_AsLong(self->badTraceCount);
    self->_statusCounter->traceCount = (int)PyLong_AsLong(self->traceCount);
    self->_statusCounter->inTraceSpanCount = (int)PyLong_AsLong(self->inTraceSpanCount);
    if (PyErr_Occurred()) {
        return;
    }

    UpdateServiceCountersFromDict(
        &self->_statusCounter->myTraceServiceCounters,
        self->_statusCounter->myTraceStatusCounter,
        self->myTraces
    );
    UpdateServiceCountersFromDict(
        &self->_statusCounter->notmyTraceServiceCounters,
        self->_statusCounter->notmyTraceStatusCounter,
        self->notmyTraces
    );
}

void _updatePyCounter(PyServiceErrorCounters* self, ServiceErrorCounters* counters) {
    self->_statusCounter = counters;
    self->ownsStatusCounter = false;
    _updatePyCounterFields(self);
}

PyMethodDef PyServiceErrorCounters_methods[] = {
    {NULL}
};

PyMemberDef PyServiceErrorCounters_members[] = {
    {"myTraces", T_OBJECT_EX, offsetof(PyServiceErrorCounters, myTraces), 0, "Counters for traces owned by this service"},
    {"notmyTraces", T_OBJECT_EX, offsetof(PyServiceErrorCounters, notmyTraces), 0, "Counters for traces owned by other services"},
    {"serviceName", T_OBJECT_EX, offsetof(PyServiceErrorCounters, serviceName), 0, "Service name"},
    {"badTraceCount", T_OBJECT_EX, offsetof(PyServiceErrorCounters, badTraceCount), 0, "Bad trace count"},
    {"traceCount", T_OBJECT_EX, offsetof(PyServiceErrorCounters, traceCount), 0, "Trace count"},
    {"inTraceSpanCount", T_OBJECT_EX, offsetof(PyServiceErrorCounters, inTraceSpanCount), 0, "In trace span count"},
    {NULL}
};

PyTypeObject PyServiceErrorCountersType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pywrapper.Counters",
    .tp_doc = "Counters objects",
    .tp_basicsize = sizeof(PyServiceErrorCounters),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyServiceErrorCounters_new,
    .tp_init = (initproc)PyServiceErrorCounters_init,
    .tp_dealloc = (destructor)PyServiceErrorCounters_dealloc,
    .tp_methods = PyServiceErrorCounters_methods,
    .tp_members = PyServiceErrorCounters_members,
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
        self->errorCounters = (PyServiceErrorCounters*)PyObject_CallObject((PyObject*)&PyServiceErrorCountersType, NULL);
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

    PyServiceErrorCounters* errorCounters = (PyServiceErrorCounters*)PyObject_CallObject((PyObject*)&PyServiceErrorCountersType, NULL);
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
        if (
            self->errorCounters->_statusCounter->myTraceStatusCounter[i] != 0 ||
            self->errorCounters->_statusCounter->notmyTraceStatusCounter[i] != 0
        ) {
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
