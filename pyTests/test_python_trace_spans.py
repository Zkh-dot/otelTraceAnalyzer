import unittest

from otelanalyzer import Analyzer, Span


TWO_SPAN_TRACE = (
    "[{'spanId': '0000000000000001', 'serviceName': 'frontend', 'traceId': '11111111111111111111111111111111'}, "
    "{'spanId': '0000000000000002', 'serviceName': 'backend', 'parentSpanId': '0000000000000001', "
    "'traceId': '11111111111111111111111111111111'}]"
)


class PythonTraceSpansTest(unittest.TestCase):
    def test_plugin_can_read_trace_spans(self):
        analyzer = Analyzer()
        seen_spans = []

        def plugin(counters, trace):
            del counters
            seen_spans.extend(
                (span.spanId, span.serviceName, span.parentSpanId, span.traceId)
                for span in trace.spansList
            )

        analyzer.plg_manager.add_plugin(plugin)
        analyzer.analyze(TWO_SPAN_TRACE, "frontend", "1" * 32)

        self.assertEqual(
            seen_spans,
            [
                ("0000000000000001", "frontend", "", "11111111111111111111111111111111"),
                ("0000000000000002", "backend", "0000000000000001", "11111111111111111111111111111111"),
            ],
        )

    def test_span_is_public_with_default_fields(self):
        span = Span()

        self.assertEqual(span.spanId, "")
        self.assertEqual(span.serviceName, "")
        self.assertEqual(span.parentSpanId, "")
        self.assertEqual(span.traceId, "")


if __name__ == "__main__":
    unittest.main()
