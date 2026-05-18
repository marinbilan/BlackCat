#!/usr/bin/env python3
"""
LabSys build / run / test orchestrator.

Usage:
    python scripts/build.py --action build   [--module NAME|all] [--build-type Release|Debug]
    python scripts/build.py --action run     [--module NAME|all]
    python scripts/build.py --action clean
    python scripts/build.py --action test    [--module NAME|all]

The script reads configs/system.json to discover modules.
All C++ work is driven through CMake + Conan.
Rust modules are handled via `cargo build`.
"""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

# ── Paths ─────────────────────────────────────────────────────────────────────
REPO_ROOT   = Path(__file__).resolve().parent.parent
BACKEND_DIR = REPO_ROOT / "backend"
RUST_DIR    = BACKEND_DIR / "rust"
BUILD_DIR   = BACKEND_DIR / "build"
BINS_DIR    = BACKEND_DIR / "bins"
CONFIG_FILE = REPO_ROOT / "configs" / "system.json"


# ── Helpers ───────────────────────────────────────────────────────────────────

def run(cmd, cwd=None, check=True):
    """Run a subprocess, stream output, and return exit code."""
    print(f"  $ {' '.join(str(c) for c in cmd)}", flush=True)
    result = subprocess.run(cmd, cwd=cwd or REPO_ROOT)
    if check and result.returncode != 0:
        sys.exit(result.returncode)
    return result.returncode


def load_config(path: Path = CONFIG_FILE) -> dict:
    with open(path) as f:
        return json.load(f)


def cpp_modules(config: dict) -> list[dict]:
    return [m for m in config["modules"] if m.get("lang", "cpp") == "cpp"]


def rust_modules(config: dict) -> list[dict]:
    return [m for m in config["modules"] if m.get("lang") == "rust"]


# ── Actions ───────────────────────────────────────────────────────────────────

def action_build(args, config: dict):
    build_type = args.build_type
    target     = args.module  # "all" or a specific module name

    # ── Conan install (C++) ────────────────────────────────────────────────
    print("\n[conan] Installing dependencies...")
    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    run([
        "conan", "install", str(BACKEND_DIR),
        "--output-folder", str(BUILD_DIR),
        "--build=missing",
        f"-s", f"build_type={build_type}",
    ])

    # ── CMake configure ────────────────────────────────────────────────────
    print("\n[cmake] Configuring...")
    toolchain = BUILD_DIR / "conan_toolchain.cmake"
    run([
        "cmake", "-B", str(BUILD_DIR), "-S", str(BACKEND_DIR),
        f"-DCMAKE_BUILD_TYPE={build_type}",
        f"-DCMAKE_TOOLCHAIN_FILE={toolchain}",
    ])

    # ── CMake build ────────────────────────────────────────────────────────
    print(f"\n[cmake] Building target={target}...")
    cmake_target = [] if target == "all" else ["--target", target]
    run(["cmake", "--build", str(BUILD_DIR), "--parallel"] + cmake_target)

    # ── Rust build ────────────────────────────────────────────────────────
    if target == "all" or any(m["name"] == target for m in rust_modules(config)):
        cargo_profile = "release" if build_type == "Release" else "dev"
        print("\n[cargo] Building Rust workspace...")
        run(["cargo", "build",
             "--profile", cargo_profile,
             "--target-dir", str(BINS_DIR / "rust")],
            cwd=RUST_DIR)


def find_nats_server():
    """Return path to nats-server, checking PATH then ~/.local/bin."""
    found = shutil.which("nats-server")
    if found:
        return found
    local = Path.home() / ".local" / "bin" / "nats-server"
    if local.exists():
        return str(local)
    return None


def action_run(args, config: dict):
    target   = args.module
    nats_url = config["system"]["nats_url"]

    modules = config["modules"]
    if target != "all":
        modules = [m for m in modules if m["name"] == target]

    enabled     = [m for m in modules if m.get("enabled", False)]
    pre_modules = [m for m in enabled if m["name"] != "runtime_manager"]
    rm_modules  = [m for m in enabled if m["name"] == "runtime_manager"]

    for m in [m for m in modules if not m.get("enabled", False)]:
        print(f"  skip (disabled): {m['name']}")

    procs = []

    # 1. nats-server — must be first
    nats_bin = find_nats_server()
    if nats_bin:
        print(f"  launch: nats-server -p 4222 -m 8222")
        procs.append(subprocess.Popen([nats_bin, "-p", "4222", "-m", "8222"]))
        time.sleep(0.5)
    else:
        print("  WARN: nats-server not found, assuming already running on 4222")

    # 2. Device modules — must subscribe to lifecycle before RM starts
    for mod in pre_modules:
        binary = BACKEND_DIR / mod["binary"]
        if not binary.exists():
            print(f"  ERROR: binary not found: {binary}")
            sys.exit(1)
        mod_args = list(mod.get("args", []))
        if "--nats-url" not in mod_args:
            mod_args = ["--nats-url", nats_url] + mod_args
        cmd = [str(binary)] + mod_args
        print(f"  launch: {' '.join(cmd)}")
        procs.append(subprocess.Popen(cmd, cwd=BACKEND_DIR))

    if pre_modules:
        time.sleep(0.5)   # give modules time to connect and subscribe

    # 3. Runtime Manager — drives lifecycle, must start last
    for mod in rm_modules:
        binary = BACKEND_DIR / mod["binary"]
        if not binary.exists():
            print(f"  ERROR: binary not found: {binary}")
            sys.exit(1)
        mod_args = list(mod.get("args", []))
        if "--nats-url" not in mod_args:
            mod_args = ["--nats-url", nats_url] + mod_args
        cmd = [str(binary)] + mod_args
        print(f"  launch: {' '.join(cmd)}")
        procs.append(subprocess.Popen(cmd, cwd=BACKEND_DIR))

    print(f"\nStarted {len(procs)} process(es). Press Ctrl-C to stop all.")
    try:
        for p in procs:
            p.wait()
    except KeyboardInterrupt:
        print("\nStopping all processes...")
        for p in reversed(procs):
            p.terminate()


def action_clean(_args, _config):
    import shutil
    print(f"[clean] Removing {BUILD_DIR}")
    shutil.rmtree(BUILD_DIR, ignore_errors=True)
    print(f"[clean] Removing {BINS_DIR}")
    shutil.rmtree(BINS_DIR, ignore_errors=True)
    print("[clean] Running cargo clean...")
    run(["cargo", "clean"], cwd=RUST_DIR, check=False)


def action_test(args, config: dict):
    target = args.module
    # CTest runs all registered GTest binaries
    ctest_filter = [] if target == "all" else ["-R", target]
    print("\n[ctest] Running C++ unit tests...")
    run(["ctest", "--test-dir", str(BUILD_DIR), "--output-on-failure"] + ctest_filter)

    if target == "all":
        print("\n[cargo test] Running Rust tests...")
        run(["cargo", "test"], cwd=RUST_DIR)


# ── Entry point ───────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="LabSys build orchestrator")
    parser.add_argument("--action",     choices=["build", "run", "clean", "test"],
                        default="build")
    parser.add_argument("--module",     default="all",
                        help="Module name or 'all'")
    parser.add_argument("--build-type", choices=["Release", "Debug"],
                        default="Release")
    parser.add_argument("--config",     default=str(CONFIG_FILE),
                        help="Path to system.json")
    args = parser.parse_args()

    config = load_config(Path(args.config))

    dispatch = {
        "build": action_build,
        "run":   action_run,
        "clean": action_clean,
        "test":  action_test,
    }
    dispatch[args.action](args, config)


if __name__ == "__main__":
    main()
