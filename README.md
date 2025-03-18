Pure C lib for python3 (or C if u wanna) to analyze traces and check for compliance with the [OpenTelemetry](https://opentelemetry.io/) standard.

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
    "myDuplicateSpan": 0,
    "notmyDuplicateSpan": 0,
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

Plugins for this package can add more robust trace analysis and edge case management. They will be called after splitting traces into spans, so you can work more comfortably. The motivation for this feature is to be able to add NDA-protected features without forking the project.

They are called during traceString analyzation. Steps are as follows:

1. Split `traceString` into spans, add span statuses, and increase error counters accordingly
2. Call all plugins
3. Free the trace object for optimization reasons

## C plugins

You can add your C analysis plugins in the `src/plugin_manager.c` file. Their source files should be placed in the `src/plugins/` folder. A plugin should have a function with `Analyzer*` and `Trace*` parameters and should return `void`. There is an example plugin in `src/plugins/example_plugin.*`

You are strongly encouraged to write your plugins with header files, as demonstrated in `example_plugin`.

Your function will be called after parsing the trace into separate spans. Please keep in mind that `trace.traceString` will be freed at this point for optimization reasons.

## Python plugins

You can add your own Python plugin to modify the existing behavior of the analyzer. To do this, create a function that takes `dict[str, Counter]` and `Trace` as input parameters. Then, pass a pointer (just the function's name without brackets or parameters) to `Analyzer.plg_manager.add_plugin(your_func)`. Plugins will be executed in a first-in, first-out order.

Your function's behaviour is not restricted in any way except for input-output types: feel free to call your custom classes, write raw data right into DB and whatever.

`dict[str, Counter]` contains service names as keys and their corresponding `ErrorCounter` values. Only services that appear at least once in the trace spans are included.

`Trace` is the current trace object being analyzed. See the plugin execution steps for reference.

IF YOU WANT TO MODIFY the `Counter` value corresponding to your service, YOU MUST return the updated `dict[str, Counter]` object. Keep in mind that the `Trace` object is read-only, so your changes will not be applied directly.

If you want to modify the `examples` property of a counter, YOU MUST also update `ExamplesCount`, or your changes will not take effect.

Plugin example:

```python
def example_plugin(d: dict[str, Counter], t: Trace):
    d["some-name"].mySpanCount = 9999999
    d["some-name"].myBadTraceExamples.append("pipa")
    d["some-name"].myExamplesCount += 1
    return d

def test_plugin(a: Analyzer, t: str):
    a.plg_manager.add_plugin(lambda_func)
```

# Installation

```$ pip install otelanalyzer```

or

```$ python3 setup.py build_ext --inplace && mv ./otelanalyzer.so ./otelanalyzer```

# Dependencies

The package has no dependencies.

# Planned features

* Python unit tests
* Wheels for Python 3.13 (you can already build it locally, check installation)

