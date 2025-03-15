class Counters:
    def __init__(self) -> None:
        ...

class Analyzer:
    def __init__(self) -> None:
        ...
    def analyze(self, trace: str, service_name: str, trace_id: str) -> None:
        ...
    def analyze_btrace(self, service_name: str) -> None:
        ...
    def get_counters(self) -> dict[str, int]:
        ...
    def get_counters_obj(self) -> Counters:
        ...
    def get_all_counters(self) -> dict[str, dict[str, int]]:
        ...
    def get_all_counters_obj(self) -> dict[str, Counters]:
        ...

class Trace:
    traceString: str
    traceId: str
    serviceName: str
    spansCount: int
    def __init__(self, trace_id: str) -> None:
        ...
    

# class Counters:
#     myBadTraceExamples: list[str]
#     notmyBadTraceExamples: list[str]
#     myExamplesCount: int
#     notmyExamplesCount: int
#     serviceName: str
#     badTraceCount: int
#     mySpanCount: int
#     traceCount: int
#     def __init__(self) -> None:
#         ...
    