# LabSys — Progress Log

## Project Overview

LabSys is a modern automation system inspired by NI LabVIEW and MATLAB Simulink.
It provides a flexible, remotely controllable platform for real and simulated lab devices
(PID regulators, signal generators, filters, recorders, …) with a web-browser front end.

---

## Session 1 — Architecture Decisions (2026-03-17)

All of the following were decided before any code was written.

### Communication planes

| Plane | Technology | Purpose |
|-------|-----------|---------|
| Control | NATS (pub/sub) | Lifecycle commands, state transitions, config distribution |
| Data | SPSC ring buffers (shared memory) | High-throughput, low-latency sample passing between devices |

### Key components agreed upon

- **Runtime Manager** — central orchestrator; owns the state machine and launches module processes.
- **Channel Registry** — singleton; created before any device; owns all named output channels.
- **Factory** — instantiates device objects inside each module process from JSON config.
- **One process per device type** — `signal_gen`, `pid_regulator`, `mod_fft`, `mod_recorder` each run as independent OS processes.
- **Rust in the same repo** — Rust modules (`mod_fft`, `mod_recorder`) live under `backend/rust/` and are built with Cargo; C++ modules use CMake + Conan 2.

### Runtime Manager state machine

```
Create  →  Init  →  Create Channels  →  Attach
```

- **Create** — RM spawns module processes; each Master creates its device objects.
- **Init** — Masters read parameters from the JSON config and initialise devices.
- **Create Channels** — Every device registers its output channels with the Channel Registry (named `OutputChannel.<Device>.<ChN>`).
- **Attach** — Every device attaches its input channels to already-registered output channels.

### Channel ownership model

```
Device0 → OutputChannel.Device0.Ch0 ──────────────────────────┐
Device0 → OutputChannel.Device0.Ch2 ──→ InputChannel.Device3.Ch0 → Device3
Device1 → OutputChannel.Device1.Ch0 ──→ InputChannel.Device3.Ch1 /
```

---

## Session 2 — Backend Skeleton (2026-03-17)

### What was created

#### Build / packaging

| File | Description |
|------|-------------|
| `backend/CMakeLists.txt` | Top-level CMake; adds all sub-directories |
| `backend/conanfile.py` | Conan 2 recipe; declares NATS.c, spdlog, nlohmann-json, GTest |
| `backend/cmake/LabSysModule.cmake` | Helper macro `labsys_module()` for consistent per-module build rules |
| `scripts/build.py` | Python helper: runs `conan install` then `cmake --build` |

#### Runtime Manager (`backend/runtime_manager/`)

| File | Status |
|------|--------|
| `if/RuntimeManagerIf.h` | Interface with four virtual lifecycle hooks + `run()` |
| `inc/RuntimeManager.h` | Concrete class; TODOs for NATS handle and module process list |
| `src/RuntimeManager.cpp` | Stub implementations of all four phases |
| `main.cpp` | Entry point: constructs `RuntimeManager`, calls `run()` |

#### Common (`backend/common/`)

| Component | Files | Status |
|-----------|-------|--------|
| Channel interface | `channel/if/ChannelIf.h` | Pure virtual: `name`, `size`, `empty`, `full`, `write`, `read` |
| SpscChannel | `channel/inc/SpscChannel.h`, `src/SpscChannel.cpp` | Cache-line-aligned `atomic<size_t>` head/tail; backed by `std::vector<std::byte>` (POSIX shm later) |
| Channel Registry | `registry/if/ChannelRegistryIf.h`, `inc/ChannelRegistry.h`, `src/ChannelRegistry.cpp` | Singleton; `create()` / `get()` / `list()` over a `string → shared_ptr<ChannelIf>` map |
| Factory | `factory/if/FactoryIf.h`, `inc/Factory.h`, `src/Factory.cpp` | Registers and instantiates device objects by string name |
| Log | `log/inc/Log.h` | `LOG_INFO` / `LOG_WARN` / `LOG_ERR` macros (thin wrapper, spdlog-ready) |

#### Module: Signal Generator (`backend/modules/signal_gen/`)

| File | Status |
|------|--------|
| `master/` (if / inc / src) | `SignalGenMaster` stub; `onCreate` / `onInit` / `onCreateChannels` / `onAttach` |
| `devices/sg_base/` | `SgBase` abstract base for all signal generator variants |
| `devices/sg_vendor_a/` | `SgVendorA` skeleton (derives `SgBase`) |
| `main.cpp` | Entry point for the `mod_signal_gen` process |

#### Module: PID Regulator (`backend/modules/pid_regulator/`)

| File | Status |
|------|--------|
| `master/` (if / inc / src) | `PidMaster` stub — same four-phase hooks |
| `devices/pid_base/` (if / inc / src) | `PidBase` skeleton with `kp`, `ki`, `kd` members |
| `main.cpp` | Entry point for the `mod_pid` process |

#### Rust modules (`backend/rust/`)

| Crate | Status |
|-------|--------|
| `mod_fft` | Cargo workspace member; `main.rs` stub (FFT placeholder) |
| `mod_recorder` | Cargo workspace member; `main.rs` stub (recorder placeholder) |

#### Configuration

| File | Description |
|------|-------------|
| `configs/system.json` | Declares all five modules with binary paths, enabled flags, NATS URL |

---

## Session 3 — NATS Control Plane + Registry Architecture (2026-03-17)

### Architecture decisions

| Topic | Decision |
|-------|----------|
| IPC protocol | **NATS only** — request/reply for registry operations; pub/sub for lifecycle events |
| Registry owner | **Runtime Manager** owns both Device Registry and Channel Registry |
| Factory | **Kept** — in-process object construction per module binary (different concern from registry) |
| Old in-process ChannelRegistry singleton | **Demoted** — removed from `LabSysCommon`; files kept in `common/registry/` for reference only |

### NATS subject namespace

```
# Lifecycle (RM → modules, pub/sub)
labsys.lifecycle.create
labsys.lifecycle.init
labsys.lifecycle.createChannels
labsys.lifecycle.attach

# Device registration (module → RM, request/reply)
labsys.registry.device.register     payload: {type, instanceName, moduleName, pid}

# Channel registration (module → RM, request/reply)
labsys.registry.channel.create      payload: {name, shm_path, capacity}

# Channel lookup (module → RM, request/reply)
labsys.registry.channel.get.<name>  reply: {shm_path, capacity} | {status: "error"}
```

### What was implemented

#### Runtime Manager — fully wired

| Addition | Detail |
|----------|--------|
| NATS connection | `natsConnection_ConnectTo()` in constructor |
| `DeviceInfo` struct | `{type, instanceName, moduleName, pid}` |
| `ChannelInfo` struct | `{name, shmPath, capacity}` |
| `m_deviceRegistry` | `unordered_map<string, DeviceInfo>` — tracks all running instances system-wide |
| `m_channelRegistry` | `unordered_map<string, ChannelInfo>` — replaces old in-process singleton |
| `onDeviceRegister` handler | Parses JSON payload, inserts into device registry, replies ACK |
| `onChannelCreate` handler | Parses JSON payload, inserts into channel registry, replies ACK |
| `onChannelGet` handler | Strips `labsys.registry.channel.get.` prefix, looks up channel, replies shm metadata |
| Lifecycle publishers | `onCreate/onInit/onCreateChannels/onAttach` each publish to their NATS topic + `nats_Sleep(500)` |

#### SignalGenMaster — fully wired

| Addition | Detail |
|----------|--------|
| NATS connection + lifecycle subscriptions | Subscribes to all 4 `labsys.lifecycle.*` subjects |
| `registerDevicesWithRM()` | For each created device: NATS request to `labsys.registry.device.register` |
| `registerChannelsWithRM()` | For each device × channel: NATS request to `labsys.registry.channel.create` |
| `attachInputChannels()` | Stub with documented NATS lookup example (SG devices have no inputs) |

#### PidMaster — fully wired

| Addition | Detail |
|----------|--------|
| Same NATS structure as SignalGenMaster | — |
| `registerChannelsWithRM()` | Registers `OutputChannel.<name>.Control` per PID instance |
| `attachInputChannels()` | Stub documenting Setpoint + Measurement lookup via NATS |

#### LabSysCommon — in-process ChannelRegistry removed

- `registry/src/*.cpp` excluded from `COMMON_SOURCES` in `common/CMakeLists.txt`
- `registry/if` and `registry/inc` removed from public include directories
- `SgBase.cpp`: removed stale `#include "ChannelRegistry.h"`; TODOs updated to reference NATS flow
- `PidBase.cpp`: TODOs updated to reference NATS flow
- `ChannelRegistryIf.h` / `ChannelRegistry.h`: marked `DEPRECATED`, explain migration to RM-owned NATS registry

---

## Current State

- **Control plane complete** — NATS lifecycle and registry flows are fully implemented end-to-end.
- `RuntimeManager` runs the full state machine and responds to all registry NATS subjects.
- `SignalGenMaster` and `PidMaster` register devices and channels with the RM via NATS on each lifecycle phase.
- `Factory` exists but device types do not yet extend `ObjectBase`; Masters still use `std::make_shared<T>` directly.
- `SpscChannel` has the correct atomic head/tail structure but ring-buffer arithmetic and `shm_open`/`mmap` are TODO.
- Channel registration sends `shm_path` metadata over NATS but the actual shared memory segments are not created yet.
- Rust crates are stub `main.rs` files.

---

## Next Steps

1. **Real SPSC ring buffer** — implement `SpscChannel::write` / `read` with correct power-of-2 masking; migrate backing store to `shm_open` / `mmap` for inter-process use.
2. **Channel shm creation** — in `SgBase::createChannels()` and `PidBase::createChannels()`: call `shm_open` + `mmap`, then report the path to the Master for NATS registration.
3. **Attach shm inputs** — in `attachInputChannels()`: use the `shm_path` returned by `labsys.registry.channel.get.*` to open and map the producer's shared memory segment.
4. **Factory device construction** — extend device base classes from `ObjectBase`; replace `std::make_shared<SgVendorA>` with `Factory::instance().create("SgVendorA")`.
5. **First real device** — bring `SgVendorA` to life: generate a sine wave and push samples to the output channel.
6. **Frontend skeleton** — React + ReactFlow graph viewer that renders device nodes from `system.json`.
7. **Build validation** — confirm CMake + Conan 2 pipeline produces all binaries end-to-end.
