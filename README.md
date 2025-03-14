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
```
$ valgrind --tool=memcheck --leak-check=full python3 test_lib.py 

==374573== HEAP SUMMARY:
==374573==     in use at exit: 397,058 bytes in 11 blocks
==374573==   total heap usage: 3,462 allocs, 3,451 frees, 4,881,009 bytes allocated
==374573== 
==374573== LEAK SUMMARY:
==374573==    definitely lost: 0 bytes in 0 blocks
==374573==    indirectly lost: 0 bytes in 0 blocks
==374573==      possibly lost: 0 bytes in 0 blocks
==374573==    still reachable: 397,058 bytes in 11 blocks
==374573==         suppressed: 0 bytes in 0 blocks
```