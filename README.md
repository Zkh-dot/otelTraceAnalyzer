Pure C lib for python3 (or C if u wanna) to analyze traces check for compliance with the [OpenTelemetry]([url](https://opentelemetry.io/)) standard.

## test:

```python
from libanalyzer import Analyzer
import json

a = Analyzer()

t = "[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]"

t_id = "0" * 32

a.analyze_trace(t, 'some-name', t_id)

r = a.get_service_error_counters('some-name')

print(json.dumps(r, indent=4))

print(type(r))
```

## result
```
$ python3 test_lib.py 
{
    "UndefTraceStatus": 0,
    "myMissingParent": 0,
    "notmyMissingParent": 0,
    "myNoParentInTrace": 1,
    "notmyNoParentInTrace": 0,
    "myDublicateSpan": 0,
    "notmyDublicateSpan": 0,
    "myBadSpanIdSize": 0,
    "notmyBadSpanIdSize": 0,
    "myBadTraceIdSize": 0,
    "notmyBadTraceIdSize": 0,
    "myExamples": [
        "00000000000000000000000000000000"
    ],
    "notmyExamples": [],
    "badTraceCount": 0,
    "mySpanCount": 1,
    "traceCount": 1
}
<class 'dict'>
```