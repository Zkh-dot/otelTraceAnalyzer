from typing import Callable
from typing import TypedDict

class TraceCounters(TypedDict):
    statusCounters: dict[str, int]
    myExamples: list[str]
    notmyExamples: list[str]
    myExamplesCount: int
    notmyExamplesCount: int
    mySpanCount: int
    notmySpanCount: int

CountersDict = TypedDict("CountersDict", {
    "my-traces": TraceCounters,
    "notmy-traces": TraceCounters,
    "badTraceCount": int,
    "traceCount": int,
    "inTraceSpanCount": int,
})

class Counters:
    myTraces: TraceCounters
    notmyTraces: TraceCounters
    serviceName: str
    badTraceCount: int
    traceCount: int
    inTraceSpanCount: int
    def __init__(self) -> None:
        ...
class Analyzer:
    plg_manager: PluginManager
    def __init__(self) -> None:
        ...
    def analyze(self, trace: str, service_name: str, trace_id: str) -> None:
        ...
    def analyze_btrace(self, service_name: str) -> None:
        ...
    def get_counters(self) -> CountersDict:
        ...
    def get_counters_obj(self) -> Counters:
        ...
    def get_all_counters(self) -> dict[str, CountersDict]:
        ...
    def get_all_counters_obj(self) -> dict[str, Counters]:
        ...
    def get_service(self, service_name: str) -> Service:
        ...
    def get_all_services(self) -> list[Service]:
        ...

class Span:
    spanId: str
    serviceName: str
    parentSpanId: str
    traceId: str
    def __init__(self) -> None:
        ...

class Trace:
    traceString: str
    traceId: str
    serviceName: str
    spansCount: int
    spansList: list[Span]
    def __init__(self, trace_id: str) -> None:
        ...

class Service:
    serviceName: str
    errorCounters: Counters
    def __init__(self) -> None:
        ...
    def is_ok(self) -> bool:
        ...

class PluginManager:
    def __init__(self) -> None:
        ...
    def add_plugin(self, func: Callable) -> None:
        ...
