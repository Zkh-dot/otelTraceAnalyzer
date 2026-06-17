import json
import tempfile
import unittest
from pathlib import Path

from otelanalyzer import Analyzer
from otelanalyzer.cli import analyze_files


class ReadySubsetTest(unittest.TestCase):
    def test_missing_service_name_counter(self):
        analyzer = Analyzer()
        trace = "[{'spanId': '0000000000000001', 'traceId': '00000000000000000000000000000000'}]"

        analyzer.analyze(trace, "svc", "1" * 32)
        counters = analyzer.get_counters("svc")

        self.assertEqual(counters["noServiceNameSpan"], 1)
        self.assertEqual(counters["myExamples"], ["1" * 32])

    def test_get_all_counters_returns_independent_dicts(self):
        analyzer = Analyzer()
        trace = "[{'spanId': '0000000000000001', 'serviceName': 'svc', 'traceId': '00000000000000000000000000000000'}]"

        analyzer.analyze(trace, "svc", "2" * 32)
        first = analyzer.get_all_counters()
        first["svc"]["mySpanCount"] = 999
        second = analyzer.get_all_counters()

        self.assertEqual(second["svc"]["mySpanCount"], 1)

    def test_cli_writes_metrics(self):
        payload = {
            "traces": [
                {
                    "3" * 32: [
                        {
                            "spanId": "0000000000000001",
                            "serviceName": "svc",
                            "traceId": "3" * 32,
                        }
                    ]
                }
            ]
        }

        with tempfile.TemporaryDirectory() as temp_dir:
            input_path = Path(temp_dir) / "svc.json"
            output_path = Path(temp_dir) / "metrics.json"
            input_path.write_text(json.dumps(payload), encoding="utf-8")

            metrics = analyze_files([input_path], output_path)
            written_metrics = json.loads(output_path.read_text(encoding="utf-8"))

        self.assertEqual(metrics, written_metrics)
        self.assertEqual(written_metrics["svc"]["traceCount"], 1)


if __name__ == "__main__":
    unittest.main()
