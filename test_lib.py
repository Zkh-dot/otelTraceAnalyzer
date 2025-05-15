from otelanalyzer import *
import json


def test(a: Analyzer, t: str = "", i: int = 0):
    t_id = "0" * 31 + str(i)
    if t == "":
        t = "[{'spanId': '0000000000000000', 'serviceName': 'some-name-" + str(i % 1000) + "', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]"

    a.analyze(t, 'some-name', t_id)

    del t
    del t_id
    # print(json.dumps(r, indent=4))

    # print(type(r))

def testTrace(a: Analyzer, t: str):
    tr = Trace()
    tr.traceString = t
    tr.traceId = "1" * 32
    tr.serviceName = "some-name"
    a.analyze_btrace(tr)
    r = a.get_counters('some-name')
    print(json.dumps(r, indent=4))

def testCounters(a: Analyzer, t: str):
    tr = Trace()
    tr.traceString = t
    tr.traceId = "1" * 32
    tr.serviceName = "some-name"
    a.analyze_btrace(tr)
    r = a.get_counters_obj('some-name')
    print(r.mySpanCount)
    

def testAllCounters(a: Analyzer, t: str):
    tr = Trace()
    tr.traceString = t
    tr.traceId = "1" * 32
    tr.serviceName = "some-name"
    a.analyze_btrace(tr)
    r = a.get_all_counters_obj()
    print(r['some-name'].mySpanCount)

def testmem(a: Analyzer, t: str):

    # test(a, t)
    # testTrace(a, t)
    # testCounters(a, t)
    from time import sleep
    sleep(10)
    print("start")
    for i in range(10000 * 10000):
        test(a, t, i)
    print("ok")

def test_service(a: Analyzer, t: str):
    a.analyze(t, 'some-name', '1' * 32)
    service = a.get_service('some-name')
    print(service.errorCounters.mySpanCount)
    print(service.serviceName)

def test_all_services(a: Analyzer, t: str):
    a.analyze(t, 'some-name', '1' * 32)
    services = a.get_all_services()
    for service in services:
        print(service.errorCounters.mySpanCount)
        print(service.serviceName)


def lambda_func(d: dict[str, Counters], t: Trace):
    print("wow!")
    d["some-name"].mySpanCount = 9999999
    d["some-name"].myBadTraceExamples.append("pipa")
    d["some-name"].myExamplesCount += 1
    return d


def lambda_func_2(d: dict[str, Counters], t: Trace):
    d["some-name"].badTraceCount = 8888
    d["some-name"].myBadTraceExamples.append("pupa")
    d["some-name"].myExamplesCount += 1
    return d

if __name__ == '__main__':
    t = "[{'spanId': '0000000000000000', 'serviceName': 'some-name', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}]"
    a = Analyzer()
    # a.plg_manager.add_plugin(lambda_func)
    # a.plg_manager.add_plugin(lambda_func_2)

    traceId = "1" * 32
    a.analyze(t, 'some-name', traceId)
    r = a.get_counters('some-name')
    
    print(json.dumps(r, indent=4))
    print(a.get_service('some-name').is_ok())