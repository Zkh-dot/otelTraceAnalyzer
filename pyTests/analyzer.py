import pytest
import analyzer
spans = [
    "{'spanId': '0000000000000000', 'serviceName': 'service1', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}"
    "{'spanId': '0000000000000001', 'serviceName': 'service1', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}"
    "{'spanId': '0000000000000002', 'serviceName': 'service2', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}"
    "{'spanId': '0000000000000003', 'serviceName': 'service2', 'parentSpanId': '0000000000000000', 'traceId': '00000000000000000000000000000000', 'project': 'some-project', 'service': 'some-service'}"

]

services = [
    {"service1": {
            "spans": [
            ],
            "expectedCounters": {}
        }
    },
    {"service2": {
            "spans": [],
            "expectedCounters": {}
        }
    }
]


def test_analyze():
    a = analyzer.Analyzer()