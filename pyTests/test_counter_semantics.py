import unittest
import subprocess
import sys
import textwrap

from otelanalyzer import Analyzer


MIXED_TRACE = (
    "[{'spanId': '0000000000000001', 'serviceName': 'frontend', 'traceId': '11111111111111111111111111111111'}, "
    "{'spanId': '0000000000000002', 'serviceName': 'backend', 'parentSpanId': '0000000000000001', "
    "'traceId': '11111111111111111111111111111111'}]"
)

BAD_NOTMY_TRACE = (
    "[{'spanId': '0000000000000001', 'serviceName': 'frontend', 'traceId': '22222222222222222222222222222222'}, "
    "{'spanId': '0000000000000002', 'serviceName': 'backend', 'parentSpanId': '0000000000009999', "
    "'traceId': '22222222222222222222222222222222'}]"
)


class CounterSemanticsTest(unittest.TestCase):
    def test_requested_service_counts_trace_and_in_trace_spans(self):
        analyzer = Analyzer()
        analyzer.analyze(MIXED_TRACE, "frontend", "1" * 32)

        frontend = analyzer.get_counters("frontend")

        self.assertEqual(frontend["traceCount"], 1)
        self.assertEqual(frontend["mySpanCount"], 1)
        self.assertEqual(frontend["inTraceSpanCount"], 2)
        self.assertEqual(frontend["myNoParentInTrace"], 0)

    def test_other_services_receive_notmy_span_count(self):
        analyzer = Analyzer()
        analyzer.analyze(MIXED_TRACE, "frontend", "1" * 32)

        backend = analyzer.get_counters("backend")

        self.assertEqual(backend["traceCount"], 0)
        self.assertEqual(backend["mySpanCount"], 0)
        self.assertEqual(backend["notmySpanCount"], 1)
        self.assertEqual(backend["notmyNoParentInTrace"], 0)
        self.assertEqual(backend["notmyExamples"], [])

    def test_other_service_parent_error_uses_notmy_counter(self):
        analyzer = Analyzer()
        analyzer.analyze(BAD_NOTMY_TRACE, "frontend", "2" * 32)

        backend = analyzer.get_counters("backend")

        self.assertEqual(backend["notmyNoParentInTrace"], 1)
        self.assertEqual(backend["notmyExamples"], ["2" * 32])

    def test_all_counters_obj_exposes_new_fields(self):
        analyzer = Analyzer()
        analyzer.analyze(MIXED_TRACE, "frontend", "1" * 32)

        counters = analyzer.get_all_counters_obj()

        self.assertEqual(counters["frontend"].traceCount, 1)
        self.assertEqual(counters["frontend"].mySpanCount, 1)
        self.assertEqual(counters["frontend"].inTraceSpanCount, 2)
        self.assertEqual(counters["backend"].traceCount, 0)
        self.assertEqual(counters["backend"].mySpanCount, 0)
        self.assertEqual(counters["backend"].notmySpanCount, 1)

    def test_all_counters_obj_teardown_does_not_crash(self):
        script = textwrap.dedent(
            f"""
            import gc
            from otelanalyzer import Analyzer

            analyzer = Analyzer()
            analyzer.analyze({MIXED_TRACE!r}, "frontend", "1" * 32)
            counters = analyzer.get_all_counters_obj()
            assert counters["frontend"].inTraceSpanCount == 2
            assert counters["backend"].notmySpanCount == 1
            del counters
            del analyzer
            gc.collect()
            """
        )

        result = subprocess.run(
            [sys.executable, "-c", script],
            text=True,
            capture_output=True,
            check=False,
        )

        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
