import argparse
import json
from pathlib import Path

from . import Analyzer


def _service_name_from_path(path: Path) -> str:
    return path.name.rsplit(".", 1)[0]


def _iter_traces(payload):
    traces = payload.get("traces", payload if isinstance(payload, list) else [])
    for trace in traces:
        if isinstance(trace, dict) and len(trace) == 1:
            trace_id, spans = next(iter(trace.items()))
            yield trace_id, str(spans)
        else:
            trace_id = trace.get("traceId", "") if isinstance(trace, dict) else ""
            yield trace_id, str(trace)


def analyze_files(paths, output_path):
    analyzer = Analyzer()
    for raw_path in paths:
        path = Path(raw_path)
        service_name = _service_name_from_path(path)
        with path.open("r", encoding="utf-8") as input_file:
            payload = json.load(input_file)
        for trace_id, trace_string in _iter_traces(payload):
            analyzer.analyze(trace_string, service_name, trace_id)

    metrics = analyzer.get_all_counters()
    with Path(output_path).open("w", encoding="utf-8") as output_file:
        json.dump(metrics, output_file, indent=4)
        output_file.write("\n")
    return metrics


def main(argv=None):
    parser = argparse.ArgumentParser(description="Analyze JSON trace files and write otelanalyzer metrics.")
    parser.add_argument("files", nargs="+", help="JSON files with a top-level traces array")
    parser.add_argument("-o", "--output", default="metrics.json", help="Output metrics JSON path")
    args = parser.parse_args(argv)

    analyze_files(args.files, args.output)
    return 0
