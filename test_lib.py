from otelanalyzer import *
import json


def test(a: Analyzer, t: str):
    t_id = "0" * 32

    a.analyze(t, 'some-name', t_id)

    r = a.get_counters('some-name')

    print(json.dumps(r, indent=4))

    print(type(r))

def testTrace(a: Analyzer, t: str):
    tr = Trace()
    tr.traceString = t
    tr.traceId = "1" * 32
    tr.serviceName = "some-name"
    a.analyze_btrace(tr)
    r = a.get_counters('some-name')
    print(json.dumps(r, indent=4))

# def testCounters(a: Analyzer, t: str):
#     tr = Trace()
#     tr.traceString = t
#     tr.traceId = "1" * 32
#     tr.serviceName = "some-name"
#     a.analyze_btrace(tr)
#     r = a.get_counters_obj('some-name')
#     print(r.myBadTraceExamples)


if __name__ == '__main__':
    t = "[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]"
    # t = "[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]"
    a = Analyzer()  
    test(a, t)
    testTrace(a, t)
    # testCounters(a, t)