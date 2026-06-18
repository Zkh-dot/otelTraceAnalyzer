#pragma once

#include "py_structs.h"
#include "../structures/counters.h"
#include "../structures/span.h"
#include "../structures/trace.h"
#include "../structures/service.h"
#include "../structures/servicemap.h"


ServiceErrorCounters* PyCounters2Counters(PyObject* src);

PyObject* Counters2Dict(ServiceErrorCounters* Counters);

// Span* PySpan2Span(PyObject* span);

// PyObject* Span2PySpan(Span* span);

Trace* PyTrace2Trace(PyObject* src);

// Service* PyService2Service(PyObject* service);

// PyObject* Service2PyService(Service* service);
