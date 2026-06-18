#!/usr/bin/env bash
set -euo pipefail

usage() {
    echo "Usage: $0 [--dry-run] [--upload]" >&2
}

dry_run=0
upload=0
wheel_dir=""
cleanup_wheel_dir=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --dry-run)
            dry_run=1
            ;;
        --upload)
            upload=1
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            usage
            exit 2
            ;;
    esac
    shift
done

run() {
    echo "+ $*"
    if [ "$dry_run" -eq 0 ]; then
        "$@"
    fi
}

cleanup() {
    if [ "$cleanup_wheel_dir" -eq 1 ] && [ -n "$wheel_dir" ] && [ -d "$wheel_dir" ]; then
        rm -rf "$wheel_dir"
    fi
}
trap cleanup EXIT

run python3 setup.py build_ext --inplace --force

if [ "$dry_run" -eq 1 ]; then
    wheel_dir="<temp-wheel-dir>"
    run python3 -m pip wheel . --no-deps -w "$wheel_dir"
    run mkdir -p dist
    run cp "$wheel_dir"/*.whl dist/
    if [ "$upload" -eq 1 ]; then
        run twine upload "$wheel_dir"/*.whl
    else
        echo "Upload skipped. Pass --upload to upload wheels built in this run."
    fi
    exit 0
fi

wheel_dir="$(mktemp -d "${TMPDIR:-/tmp}/otelanalyzer-wheels.XXXXXX")"
cleanup_wheel_dir=1
run python3 -m pip wheel . --no-deps -w "$wheel_dir"

produced_wheels=("$wheel_dir"/*.whl)
if [ ! -e "${produced_wheels[0]}" ]; then
    echo "No wheels were produced in $wheel_dir" >&2
    exit 1
fi

run mkdir -p dist
run cp "${produced_wheels[@]}" dist/

if [ "$upload" -eq 1 ]; then
    run twine upload "${produced_wheels[@]}"
else
    echo "Upload skipped. Pass --upload to upload wheels built in this run."
fi
