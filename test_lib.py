from analyzer import Analyzer
import json
def test():
    a = Analyzer()

    t = "[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]"

    t_id = "0" * 32

    a.analyze_trace(t, 'some-name', t_id)

    r = a.get_service_error_counters('some-name')

    print(json.dumps(r, indent=4))

    print(type(r))

if __name__ == '__main__':
    test()