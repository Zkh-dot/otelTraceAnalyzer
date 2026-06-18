import contextlib
import io
import unittest

from otelanalyzer import Analyzer


GOOD_PARENT_TRACE = (
    "[{'spanId': '0000000000000001', 'serviceName': 'svc', 'traceId': '11111111111111111111111111111111'}, "
    "{'spanId': '0000000000000002', 'serviceName': 'svc', 'parentSpanId': '0000000000000001', "
    "'traceId': '11111111111111111111111111111111'}]"
)

BAD_PARENT_TRACE = (
    "[{'spanId': '0000000000000001', 'serviceName': 'svc', 'traceId': '22222222222222222222222222222222'}, "
    "{'spanId': '0000000000000002', 'serviceName': 'svc', 'parentSpanId': '0000000000009999', "
    "'traceId': '22222222222222222222222222222222'}]"
)


class SpanParserParentLookupTest(unittest.TestCase):
    def test_existing_parent_is_not_reported_missing(self):
        analyzer = Analyzer()
        analyzer.analyze(GOOD_PARENT_TRACE, "svc", "1" * 32)

        counters = analyzer.get_counters("svc")

        self.assertEqual(counters["my-traces"]["statusCounters"]["myNoParentInTrace"], 0)
        self.assertEqual(counters["my-traces"]["mySpanCount"], 2)

    def test_missing_parent_is_reported_once(self):
        analyzer = Analyzer()
        analyzer.analyze(BAD_PARENT_TRACE, "svc", "2" * 32)

        counters = analyzer.get_counters("svc")

        self.assertEqual(counters["my-traces"]["statusCounters"]["myNoParentInTrace"], 1)
        self.assertEqual(counters["my-traces"]["myExamples"], ["2" * 32])

    def test_analyze_does_not_print_trace_debug_output(self):
        analyzer = Analyzer()
        output = io.StringIO()

        with contextlib.redirect_stdout(output):
            analyzer.analyze(GOOD_PARENT_TRACE, "svc", "3" * 32)

        self.assertEqual(output.getvalue(), "")


if __name__ == "__main__":
    unittest.main()
