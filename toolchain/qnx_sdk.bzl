"""Repository rule + module extension that locates the QNX SDP at build time.

Instead of hardcoding a path, this reads environment variables that are
already set by the standard `qnxsdp-env.sh` script shipped with the SDP.

Priority order:
  1. QNX_ROOT          – direct path to the SDP root
  2. QNX_TARGET / QNX_HOST – derived (set automatically by qnxsdp-env.sh)
  3. Default fallback   – $HOME/qnx800  (common install location)
"""

# Default SDP directory name (appended to $HOME).
_DEFAULT_QNX_DIR = "qnx800"

def _qnx_sdk_repo_impl(repository_ctx):
    qnx_root = repository_ctx.os.environ.get("QNX_ROOT", "")

    if not qnx_root:
        qnx_target = repository_ctx.os.environ.get("QNX_TARGET", "")
        if qnx_target:
            # QNX_TARGET is typically <sdp>/target/qnx
            qnx_root = qnx_target.removesuffix("/target/qnx")

    if not qnx_root:
        qnx_host = repository_ctx.os.environ.get("QNX_HOST", "")
        if qnx_host:
            # QNX_HOST is typically <sdp>/host/linux/x86_64
            qnx_root = qnx_host.removesuffix("/host/linux/x86_64")

    # Fallback: try $HOME/qnx800
    if not qnx_root:
        home = repository_ctx.os.environ.get("HOME", "")
        if home:
            candidate = "{}/{}".format(home, _DEFAULT_QNX_DIR)
            result = repository_ctx.execute(["test", "-d", candidate])
            if result.return_code == 0:
                qnx_root = candidate

    if not qnx_root:
        fail(
            "Could not locate the QNX SDP.\n" +
            "Either set QNX_ROOT, source qnxsdp-env.sh, or install the SDP to $HOME/" + _DEFAULT_QNX_DIR,
        )

    repository_ctx.file("BUILD.bazel", "")
    repository_ctx.file(
        "defs.bzl",
        'QNX_ROOT = "{}"\n'.format(qnx_root),
    )

qnx_sdk_repo = repository_rule(
    implementation = _qnx_sdk_repo_impl,
    environ = ["QNX_ROOT", "QNX_TARGET", "QNX_HOST", "HOME"],
    local = True,  # always re-run; the SDK is a local, non-hermetic input
)

# ── Bzlmod module extension ────────────────────────────────────────────────

def _qnx_sdk_extension_impl(_module_ctx):
    qnx_sdk_repo(name = "qnx_sdk")

qnx_sdk = module_extension(
    implementation = _qnx_sdk_extension_impl,
)
