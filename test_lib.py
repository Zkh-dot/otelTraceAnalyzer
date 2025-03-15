from otelanalyzer import *
import json
def test():
    a = Analyzer()

    t = "[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]"

    t_id = "0" * 32

    a.analyze(t, 'some-name', t_id)

    r = a.get_counters('some-name')

    print(json.dumps(r, indent=4))

    print(type(r))

def testTrace():
    t = Trace()

if __name__ == '__main__':
    test()
    testTrace()