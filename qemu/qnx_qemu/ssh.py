"""
SSH key provider — detects or generates an SSH public key.

Implements ISshKeyProvider.
"""

from __future__ import annotations

import subprocess
from pathlib import Path

from .interfaces import ISshKeyProvider
from .logger import ConsoleLogger


class SshKeyProvider(ISshKeyProvider):
    """Locate an existing SSH key or generate a new ed25519 key pair."""

    _KEY_CANDIDATES = ("id_ed25519.pub", "id_rsa.pub")

    def __init__(self) -> None:
        self._ssh_dir = Path.home() / ".ssh"

    # ── ISshKeyProvider ─────────────────────────────────────────────────

    def public_key_path(self) -> Path:
        # Try existing keys
        for name in self._KEY_CANDIDATES:
            candidate = self._ssh_dir / name
            if candidate.is_file():
                return candidate

        # Generate a new key pair
        ConsoleLogger.info("SSH", "No public key found — generating one …")
        self._ssh_dir.mkdir(parents=True, exist_ok=True)
        private_key = self._ssh_dir / "id_ed25519"
        subprocess.run(
            [
                "ssh-keygen", "-q",
                "-t", "ed25519",
                "-N", "",
                "-f", str(private_key),
            ],
            stdin=subprocess.DEVNULL,
            check=True,
        )
        return self._ssh_dir / "id_ed25519.pub"
