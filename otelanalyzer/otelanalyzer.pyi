from typing import Callable

class Counters:
    myBadTraceExamples: list[str]
    notmyBadTraceExamples: list[str]
    myExamplesCount: int
    notmyExamplesCount: int
    serviceName: str
    badTraceCount: int
    mySpanCount: int
    traceCount: int
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
    def get_counters(self) -> dict[str, int]:
        ...
    def get_counters_obj(self) -> Counters:
        ...
    def get_all_counters(self) -> dict[str, dict[str, int]]:
        ...
    def get_all_counters_obj(self) -> dict[str, Counters]:
        ...
    def get_service(self, service_name: str) -> Service:
        ...
    def get_all_services(self) -> list[Service]:
        ...

class Trace:
    traceString: str
    traceId: str
    serviceName: str
    spansCount: int
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