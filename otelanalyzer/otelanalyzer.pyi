class Analyzer:
    def __init__(self) -> None:
        ...
    def analyze_trace(self, trace: str, service_name: str, trace_id: str) -> None:
        ...
    def get_service_error_counters(self, service_name: str) -> dict[str, int]:
        ...
    def get_all_service_error_counters(self) -> dict[str, dict[str, int]]:
        ...

class Trace:
    traceString: str
    traceId: str
    serviceName: str
    spansCount: int
    def __init__(self, trace_id: str) -> None:
        ...
    