from otelanalyzer import Analyzer
import json
import sys
import os
from os.path import basename

def get_traces(a: Analyzer, service: str, filename: str):
    with open(filename, "r") as f:
        traces = json.load(f)
        # print(traces)
        traces = traces["traces"]
        # print("--->",traces)
    for trace in traces:
        str_trace = str(trace)#.replace("'", '"')
        # print(service, ':', str_trace)
        a.analyze(str_trace, service, list(trace.keys())[0])
        # print("analyzed", f"'{service}'")

if __name__ == "__main__":
    a = Analyzer()

    args = sys.argv[1:]

    for filename in args:
        servicename = ".".join(os.path.basename(filename).split(".")[:-1])
        # print("analyzing", f"'{servicename}'")
        get_traces(a, servicename, filename)
    # args.append("some_new_service")
    # print("args:", args)

    with open("metrics.json", "w") as f:
        metrics = a.get_all_counters()
        json.dump(metrics, f, indent=4)
