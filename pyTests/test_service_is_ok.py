import unittest

from otelanalyzer import Analyzer, Service


ROOT_ONLY_TRACE = "[{'spanId': '0000000000000001', 'serviceName': 'svc', 'traceId': '11111111111111111111111111111111'}]"
BAD_PARENT_TRACE = (
    "[{'spanId': '0000000000000001', 'serviceName': 'svc', 'traceId': '22222222222222222222222222222222'}, "
    "{'spanId': '0000000000000002', 'serviceName': 'svc', 'parentSpanId': '0000000000009999', "
    "'traceId': '22222222222222222222222222222222'}]"
)
MISSING_SERVICE_TRACE = "[{'spanId': '0000000000000001', 'traceId': '33333333333333333333333333333333'}]"


class ServiceIsOkTest(unittest.TestCase):
    def test_new_service_without_analyzer_returns_true(self):
        self.assertIs(Service().is_ok(), True)

    def test_empty_analyzer_service_returns_true(self):
        self.assertIs(Analyzer().get_service("svc").is_ok(), True)

    def test_service_survives_analyzer_deallocation(self):
        analyzer = Analyzer()
        analyzer.analyze(BAD_PARENT_TRACE, "svc", "4" * 32)
        service = analyzer.get_service("svc")
        self.assertIs(service.is_ok(), False)
        del analyzer
        self.assertIs(service.is_ok(), False)

    def test_ok_service_returns_true(self):
        analyzer = Analyzer()
        analyzer.analyze(ROOT_ONLY_TRACE, "svc", "1" * 32)

        service = analyzer.get_service("svc")

        self.assertIs(service.is_ok(), True)

    def test_bad_parent_returns_false(self):
        analyzer = Analyzer()
        analyzer.analyze(BAD_PARENT_TRACE, "svc", "2" * 32)

        service = analyzer.get_service("svc")

        self.assertIs(service.is_ok(), False)

    def test_missing_service_name_returns_false(self):
        analyzer = Analyzer()
        analyzer.analyze(MISSING_SERVICE_TRACE, "svc", "3" * 32)

        service = analyzer.get_service("svc")

        self.assertIs(service.is_ok(), False)


if __name__ == "__main__":
    unittest.main()
