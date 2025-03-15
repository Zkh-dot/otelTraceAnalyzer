#pragma once

#include "py_structs.h"
#include "../structures/counters.h"
#include "../structures/span.h"
#include "../structures/trace.h"
#include "../structures/service.h"
#include "../structures/servicemap.h"


ServiceErrorCounters* PyCounters2Counters(PyObject* counters);

PyObject* Counters2PyCounters(ServiceErrorCounters* counters);

// Span* PySpan2Span(PyObject* span);

// PyObject* Span2PySpan(Span* span);

Trace* PyTrace2Trace(PyObject* trace);

PyObject* Trace2PyTrace(Trace* trace);

// Service* PyService2Service(PyObject* service);

// PyObject* Service2PyService(Service* service);
