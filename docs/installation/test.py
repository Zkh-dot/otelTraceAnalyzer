from analyzer import *
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