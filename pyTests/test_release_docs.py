import os
import subprocess
import tempfile
import textwrap
import unittest
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[1]
RELEASE_SH = REPO_ROOT / "release.sh"


class ReleaseDocsTest(unittest.TestCase):
    def test_release_script_has_valid_bash_syntax(self):
        subprocess.run(["bash", "-n", str(RELEASE_SH)], check=True)

    def test_dry_run_echoes_without_executing_commands(self):
        with ReleaseScriptHarness() as harness:
            result = harness.run("--dry-run", "--upload")

            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn("+ python3 setup.py build_ext --inplace --force", result.stdout)
            self.assertIn("+ python3 -m pip wheel . --no-deps -w <temp-wheel-dir>", result.stdout)
            self.assertIn("+ twine upload <temp-wheel-dir>/*.whl", result.stdout)
            self.assertEqual(harness.executed_commands(), [])
            self.assertFalse((harness.work_dir / "dist").exists())

    def test_dry_run_does_not_delete_placeholder_named_directory(self):
        with ReleaseScriptHarness() as harness:
            placeholder_dir = harness.work_dir / "<temp-wheel-dir>"
            placeholder_dir.mkdir()
            sentinel = placeholder_dir / "sentinel"
            sentinel.write_text("keep", encoding="utf-8")

            result = harness.run("--dry-run")

            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertTrue(sentinel.exists())
            self.assertEqual(sentinel.read_text(encoding="utf-8"), "keep")

    def test_normal_run_builds_wheel_without_uploading(self):
        with ReleaseScriptHarness() as harness:
            result = harness.run()

            self.assertEqual(result.returncode, 0, result.stderr)
            commands = harness.executed_commands()
            self.assertIn("python3 setup.py build_ext --inplace --force", commands)
            self.assertTrue(
                any(command.startswith("python3 -m pip wheel . --no-deps -w ") for command in commands),
                commands,
            )
            self.assertFalse(any(command.startswith("twine upload") for command in commands), commands)
            self.assertEqual([path.name for path in (harness.work_dir / "dist").glob("*.whl")], [harness.wheel_name])

    def test_upload_only_uploads_wheels_from_current_run(self):
        with ReleaseScriptHarness() as harness:
            dist_dir = harness.work_dir / "dist"
            dist_dir.mkdir()
            stale_wheel = dist_dir / "stale-0.0.1-py3-none-any.whl"
            stale_wheel.write_text("stale", encoding="utf-8")

            result = harness.run("--upload")

            self.assertEqual(result.returncode, 0, result.stderr)
            upload_commands = [
                command for command in harness.executed_commands() if command.startswith("twine upload")
            ]
            self.assertEqual(len(upload_commands), 1, harness.executed_commands())
            upload_command = upload_commands[0]
            self.assertIn(harness.wheel_name, upload_command)
            self.assertNotIn(str(stale_wheel), upload_command)
            self.assertNotIn("stale-0.0.1-py3-none-any.whl", upload_command)

    def test_unknown_args_fail_with_exit_code_2(self):
        with ReleaseScriptHarness() as harness:
            result = harness.run("--bogus")

            self.assertEqual(result.returncode, 2)
            self.assertIn("Usage:", result.stderr)
            self.assertEqual(harness.executed_commands(), [])


class ReleaseScriptHarness:
    wheel_name = "otelanalyzer-0.0.0-py3-none-any.whl"

    def __enter__(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.work_dir = Path(self.temp_dir.name)
        self.bin_dir = self.work_dir / "bin"
        self.log_path = self.work_dir / "commands.log"
        self.bin_dir.mkdir()
        self._write_stub_commands()
        return self

    def __exit__(self, exc_type, exc, traceback):
        self.temp_dir.cleanup()

    def run(self, *args):
        env = {
            **os.environ,
            "PATH": f"{self.bin_dir}:{os.environ['PATH']}",
            "RELEASE_TEST_LOG": str(self.log_path),
            "RELEASE_TEST_WHEEL_NAME": self.wheel_name,
        }
        return subprocess.run(
            [str(RELEASE_SH), *args],
            cwd=self.work_dir,
            env=env,
            text=True,
            capture_output=True,
        )

    def executed_commands(self):
        if not self.log_path.exists():
            return []
        return self.log_path.read_text(encoding="utf-8").splitlines()

    def _write_stub_commands(self):
        self._write_executable(
            "python3",
            """
            #!/usr/bin/env bash
            set -euo pipefail

            echo "python3 $*" >> "$RELEASE_TEST_LOG"

            if [ "$#" -ge 5 ] && [ "$1" = "-m" ] && [ "$2" = "pip" ] && [ "$3" = "wheel" ]; then
                wheel_dir=""
                previous_arg=""
                for arg in "$@"; do
                    if [ "$previous_arg" = "-w" ]; then
                        wheel_dir="$arg"
                    fi
                    previous_arg="$arg"
                done

                mkdir -p "$wheel_dir"
                printf "wheel" > "$wheel_dir/$RELEASE_TEST_WHEEL_NAME"
            fi
            """,
        )
        self._write_executable(
            "twine",
            """
            #!/usr/bin/env bash
            set -euo pipefail

            echo "twine $*" >> "$RELEASE_TEST_LOG"
            """,
        )

    def _write_executable(self, name, content):
        path = self.bin_dir / name
        path.write_text(textwrap.dedent(content).lstrip(), encoding="utf-8")
        path.chmod(0o755)


if __name__ == "__main__":
    unittest.main()
