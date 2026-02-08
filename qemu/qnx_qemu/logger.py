"""
Console logger — thin wrapper producing the same styled output
as the original shell scripts (banner lines, step numbers, etc.).
"""

from __future__ import annotations

import sys


class ConsoleLogger:
    """Structured console output matching the original shell-script style."""

    _RULE = "─" * 68
    _DOUBLE_RULE = "═" * 67

    # ── Public API ──────────────────────────────────────────────────────

    @staticmethod
    def banner(title: str) -> None:
        print(f"── {title} " + "─" * max(0, 64 - len(title)))

    @staticmethod
    def info(label: str, value: str) -> None:
        print(f"  {label + ':':14s}{value}")

    @staticmethod
    def step(number: int, total: int, message: str) -> None:
        print(f"\nStep {number}/{total}: {message} ...\n")

    @staticmethod
    def success(message: str) -> None:
        print(message)

    @staticmethod
    def warning(message: str) -> None:
        print(f"  WARNING: {message}")

    @staticmethod
    def ssh_instructions(ssh_port: int, binary_name: str, staged_tests: list | None = None) -> None:
        print()
        print(ConsoleLogger._DOUBLE_RULE)
        print("  After boot, SSH into the QNX VM (from another terminal):")
        print()
        print(f"    ssh -o StrictHostKeyChecking=no -p {ssh_port} root@localhost")
        print()
        print("  Then run the demo:")
        print()
        print(f"    /data/home/root/{binary_name}")
        if staged_tests:
            print()
            print(f"  Run tests ({len(staged_tests)} staged):")
            print()
            for t in staged_tests:
                print(f"    /data/home/root/tests/{t.name}")
        print()
        print("  To exit QEMU console: Ctrl-A X")
        print(ConsoleLogger._DOUBLE_RULE)
        print()

    @staticmethod
    def rule() -> None:
        print(ConsoleLogger._RULE)

    @staticmethod
    def error(message: str) -> None:
        print(f"ERROR: {message}", file=sys.stderr)

    @staticmethod
    def error_hint(hint: str) -> None:
        print(f"       {hint}", file=sys.stderr)
