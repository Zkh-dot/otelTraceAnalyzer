Pure C lib for python3 (or C if u wanna) to analyze traces check for compliance with the [OpenTelemetry]([url](https://opentelemetry.io/)) standard.

# Example:

```python
from otelanalyzer import *
import json

def testTrace(a: Analyzer, t: str):
    tr = Trace()
    tr.traceString = t
    tr.traceId = "1" * 32
    tr.serviceName = "some-name"
    a.analyze_btrace(tr)
    r = a.get_counters('some-name')
    print(json.dumps(r, indent=4))

if __name__ == '__main__':
    t = "[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]"
    a = Analyzer()
    testTrace(a, t)
```

## Result
```
$ python3 test_lib.py 
{
    "UndefTraceStatus": 0,
    "myMissingParent": 0,
    "notmyMissingParent": 0,
    "myNoParentInTrace": 0,
    "notmyNoParentInTrace": 0,
    "myDublicateSpan": 0,
    "notmyDublicateSpan": 0,
    "myBadSpanIdSize": 0,
    "notmyBadSpanIdSize": 0,
    "myBadTraceIdSize": 0,
    "notmyBadTraceIdSize": 0,
    "myExamples": [],
    "notmyExamples": [],
    "badTraceCount": 0,
    "mySpanCount": 1,
    "traceCount": 1
}

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

# Plugins

Plugins for this pacage can add some more robust trace analyzation and edgecase managin. They will be called after splitting trace into spans, so you can work more comfortable. Motivation for this feature is to be able to add NDA-protected features without forking project.

## C plugins

You can add your C analyzation plugins in src/plugins.c file. they src files should be placed in src/plugins/ folder. plugin should have a function Analyzer*, Trace* params that has a void type. there is an example plugin in src/plugins/example_plugin.*

You are strongly encouraged to write your plugins with header files, as example_plugin.

Your function will be called after parsing trace into separate spans. please keep in mind, that trace.traceString will be freed at this point for optimization reasons.

## Python plugins

not implemented yet, planned asap

# installation

```$ pip install otelanalyzer```

or

```$ python3 setup.py build_ext --inplace && mv ./otelanalyzer.so ./otelanalyzer```

# dependencies

Pacage has no dependencies

# planed features

* python plugins support
* python unit tests
* wheels for python-13 (you already can build it locally, check installation)
